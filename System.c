#include "System.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Job.h"
#include "command.h"


struct System* newSystem(struct Command* c){
    //crete a new system
    struct System* system = (struct System*)malloc(sizeof(struct System));

    //initialize
    system->time = 0;
    system ->totalMemory = -1;
    system->curMemory = -1;
    system -> totalDevice = -1;
    system-> curDevice = -1;
    system -> timeQuantum = -1;
    system -> holdQueue1 = newQueue(1); //SJF
    system -> holdQueue2 = newQueue(2); //FIFO
    system->waitQueue = newQueue(-1);
    system-> readyQueue = newQueue(-1);
    system->leaveQueue= newQueue(-1);
    system->running =0;
    system->startTime = 0;

    return system;
};

//Meant to check if next event was internal or external?
int nextEvent(struct System* s, struct Command* c){
    //check if job in hold queue
    if((s->holdQueue1->size != 0) || (s->holdQueue2->size != 0)){
        arriveJob(s,s->holdQueue1->head); 
        return 0; //success
        //return EVENT_ARRIVE_JOB
    }

    //check is job in ready queue
    if((s->readyQueue->size != 0)){
        moveReadyToRunning(s);
        return 0; //success 
        // return EVENT_MOVE_READY_TO_RUNNING
    }

    //check is job in waiting queue 
    if((s->waitQueue->size != 0)){
        moveWaitToReady(s);
        return 0; //success
        //return EVENT_MOVE_WAIT_TO_READY;
    }

    //check if running job has completed
    if(s->running != NULL && s->time - s->startTime >= (s->running->leaveTime - s->running->arrivalTime)){
        jobComplete(s);
        return 0;
        //return EVENT_JOB_COMPLETE;
    }

    return -1;
};

int arriveJob(struct System* s, struct Job* j){
    // find what hold queue to push the job
    if(j->priority == 1){
        pushQueue(s->holdQueue1, j);
    }
    else if(j->priority == 2){
        pushQueue(s->holdQueue2, j);
    }
    else{
        return -1; //invalid
    }
    return 0; //success

};

void scheduleQueue(struct System* s){
    //move jobs from hold to ready queue
    while((s->holdQueue1->size != 0)){
        struct Job* j = popQueue(s->holdQueue1);
        pushQueue(s->readyQueue, j);
    }

    while((s->holdQueue2->size != 0)){
        struct Job* j = popQueue(s->holdQueue2);
        pushQueue(s->readyQueue, j);
    }
};

void moveOutHold(struct System* s){
    //move jobs from hold to leave queue
    while ((s->holdQueue1->size != 0)){
            struct Job* j = popQueue(s->holdQueue1);
            pushQueue(s->leaveQueue, j);
        }
        
        while ((s->holdQueue2->size != 0)) {
            struct Job* j = popQueue(s->holdQueue2);
            pushQueue(s->leaveQueue, j);
        }
};

void moveReadyToRunning(struct System* s){

    //roudn robin way?
      // Check if a job is running on the CPU
    if (s->running != NULL) {
        // Execute the job for the time quantum
        s->running->leftTime -= s->timeQuantum;

        // Check if the job has completed
        if (s->running->leftTime <= 0) {
            // Set the leave time for the completed job
            s->running->leaveTime = s->time;

            // Move the completed job to the leave queue
            pushQueue(s->leaveQueue, s->running);
            s->running = NULL;
        } else {
            // Move the job to the end of the ready queue
            pushQueue(s->readyQueue, s->running);
            s->running = NULL;
        }
    }
};


void jobComplete(struct System* s){
    //move completed job from running state to leave queue
    if(s->running != NULL){
        pushQueue(s->leaveQueue, s->running);
        s->running = NULL;
    }

};

void moveWaitToReady(struct System* s){
    //move job from wait queue to ready queue
    if ((s->waitQueue->size != 0)) {
        struct Job* j = popQueue(s->waitQueue);
        pushQueue(s->readyQueue, j);
    }
};

void moveRunningToReady(struct System* s){
    //move running job to ready queue
    if (s->running != NULL) {
        pushQueue(s->readyQueue, s->running);
        s->running = NULL;
    }

};

void moveRunningToWait(struct System* s){
    //move running job to wait queue
    if (s->running != NULL) {
        pushQueue(s->waitQueue, s->running);
        s->running = NULL;
    }
};

void requestDevice(struct System* s, struct Command* c, int numProcesses){
    //Use bankers to check if it is safe to request device with number of processes
    int safe = bankers(s,c,numProcesses);
    if(safe == 1){
        c->devices += s -> curDevice;
        s->curDevice -= c->devices;
        pushQueue(s->readyQueue, s->running);
         }
    else{
        //if no availible devices push job to wait queue
        pushQueue(s->waitQueue, s->running); //this might wrong
    }
};

void releaseDevice(struct System* s, struct Command* c){
    //release device and icnrement availible device count
    s->totalDevice += c->devices;
    s->running->holdDevice -= c->devices;

    pushQueue(s->readyQueue,s->running);
    s->running = NULL;
};


int bankers(struct System* s, struct Command* c, int numProcesses) {
    int *allocated = malloc(numProcesses * sizeof(int));
    int *max = malloc(numProcesses * sizeof(int));
    int *need = malloc(numProcesses * sizeof(int));
    int *finish = malloc(numProcesses * sizeof(int));

    //set allocated, max, and need lists 
    struct Job* job = s->readyQueue->head;
    int totalAllocated =0;
    for(int i = 0; i <= numProcesses; i++){
        allocated[i] = job->holdDevice;
        max[i] = job->requestDevice;
        need[i] = max[i] - allocated[i];
        totalAllocated +=  allocated[i];
        job = job -> next;
    }


    int availible = s->totalDevice - totalAllocated;
    
    int work = availible;

    for (int i = 0; i <= numProcesses; i++) {
        finish[i] = 0;
    }

    // Check if the requested resources can be granted
    for (int i = 0; i <= numProcesses; i++) {
        if (need[i] > work) {
            free(allocated);
            free(max);
            free(need);
            free(finish);

            return 0;
        }
    }

    // Check for a safe state
    for(int i = 0;i < numProcesses; i++){
        for(int j = 0;j<numProcesses;j++){
            if(finish[j] == 0 && need[j] <= work){
                work += allocated[j];
                finish[j] = 1;
            }
        }
    }

    for(int i = 0;i < numProcesses; i++){
        if(finish[i] == 0){
            printf("unsafe state");

            //frees memory before jumping out
            free(allocated);
            free(max);
            free(need);
            free(finish);

            return 1;
        }
    }

    free(allocated);
    free(max);
    free(need);
    free(finish);

    return 0;
}


int printAtTime(struct System *s, int availableMem, int availableDevices)
// Prints current status of scheduler at a given time.
{
    int sum = 0;
    int count = 0;
    printf("At Time %d: \nCurrent Available Main Memory=%d \nCurrent Devices=%d \n", s->time, s->totalMemory-availableMem, s->totalDevice-availableDevices);
    printf("---------------------------------------------------------------------------\n");
    // Prints all of the finish jobs
    printf("Completed Jobs:\n");
    struct Job *job = s->leaveQueue->head;
    while (job != NULL)
    {
        sum = sum + (job->leaveTime - job->arrivalTime);
        count++;
        printf("Job ID: %d Arrival Time: %d Finish Time: %d Turn Around Time: %d\n",
               job->jobId, job->arrivalTime, job->leaveTime, (job->leftTime - job->arrivalTime));

        job = job->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all currently Hold Queue 1
    printf("Hold Queue 1: \n --------------------\n");
    job = s->holdQueue1->head;
    while (job != NULL)
    {
        printf("Job ID: %d Run Time: %d \n", job->jobId, job->burstTime);
        job = job->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all currently on Hold Queue 2
    printf("Hold Queue 2: \n --------------------\n");
    job = s->holdQueue2->head;
    while (job != NULL)
    {
        printf("Job ID: %d Run Time: %d \n", job->jobId, job->burstTime);
        job = job->next;
    }

    printf("---------------------------------------------------------------------------\n");
    //Prints all currently in Ready Queue
    printf("Ready Queue: \n ---------------------------------\n");
    job = s->readyQueue->head;
    while (job != NULL)
    {
        printf("Job ID: %d Run Time: %d Time Accrued: %d\n", job->jobId, job->burstTime, job->totalTime+(s->time-job->leftTime));
        job = job->next;
    }
    printf("---------------------------------------------------------------------------\n");
    //Prints waiting queue
    printf("Wait Queue: \n ---------------------------------\n");
    job = s->waitQueue->head;
    while (job != NULL)
    {
        printf("Job ID: %d Run Time: %d Time Accrued: %d\n", job->jobId, job->burstTime, job->totalTime+(s->time-job->leftTime));
        job = job->next;
    }
    printf("---------------------------------------------------------------------------\n");
    //Prints whats running on the CPU 
    printf("Running on CPU: \n---------------------------------\n");
    job = s->running;
    if(job != NULL){
        if(job->leftTime!=0){
    printf("Job ID: %d Time Accrued: %d Time Left: %d\n", job->jobId, job->totalTime+(s->time-job->leftTime), (job->burstTime - (job->totalTime+(s->time-job->leftTime))));
        }else{
           printf("Job ID: %d Time Accrued: %d Time Left: %d\n", job->jobId, job->totalTime+(s->time-job->arrivalTime), (job->burstTime - (job->totalTime+(s->time-job->arrivalTime)))); 
        }
    }
    printf("---------------------------------------------------------------------------\n");
    // Calculates the average turnaround time of the jobs on the finished queue
    float turnaround=0.0;
    if(count>0){
        turnaround=(float)sum/count;
    }
    printf("System Turnaround Time: %.2f\n\n",turnaround);
    return 0;
}

