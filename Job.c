#include <stdio.h>
#include <stdlib.h>
#include "Job.h"
#include "command.h"

//creates a new job from a command
struct Job* newJob(struct Command* c){
    struct Job* newJob = (struct Job*)malloc(sizeof(struct Job));
    //does set job based on command
    if(newJob == NULL){
        return NULL;
    }
    newJob->next = NULL;
    return newJob;
}

//display details of a job
void displayJob(struct Job* j, int type){
    if(type == 1){
        printf("Type: SJF");
    }
    else if (type == 2){
        printf("Typle: FIFO");
    }
    else{
        printf("Invalid");
    }
}

//creates a new queue of jobs
struct Queue* newQueue(int type){
    struct Queue* newQueue = (struct Queue*)malloc(sizeof(struct Queue));
    if ( newQueue == NULL){
        return NULL;
    }
    newQueue->head = NULL;
    newQueue->tail = NULL;
    newQueue-> queueType = type;
    return newQueue;
}



//push job to queue
void pushQueue(struct Queue* q, struct Job* j){
    if(q->head == NULL){
        q->head = j;
        q->tail = j;
        return;
    }
    if(q->queueType == 1){ // 1: SJF 2:FIFO
        struct Job* s = q->head;
        //need to find right position to intsert
        while(s->next != NULL){
            if(j->totalTime >= s-> totalTime && j->totalTime < s->next->totalTime){
                j->next = s->next;
                s->next = j;
                return;
            }
            s = s->next;
        }
        q->tail->next=j;
        q->tail = j;
    }
    else if(q->queueType == 2){ // 1: SJF 2:FIFO
        q->tail->next = j;
        q->tail=j;
    }
}

//removes the first job from a queue
struct Job* popQueue(struct Queue* q){
    if (q == NULL){
        return NULL;
    }
    struct Job* remove = q->head;
    q->head= q->head->next;
    remove->next=NULL;
    if(q->head == NULL){
        q->tail = NULL;
    }
    return remove;
}

//check if queue is empty
int emptyQueue(struct Queue*q){
    if(q->head == NULL){
        return 1;
    }
    return 0;
}

int printAtTime(int used_devices,int used_memory,int time,int time_passed, int memory, int devices, struct Queue *hold_queue1, struct Queue *hold_queue2, struct Queue *ready_queue, struct Queue *wait_queue, struct Job *finished, struct Queue *onCPU)
// Prints current status of scheduler at a given time.
{
    int sum = 0;
    int count = 0;
    printf("At Time %d: \nCurrent Available Main Memory=%d \nCurrent Devices=%d \n", time, memory-used_memory, devices-used_devices);
    printf("---------------------------------------------------------------------------\n");
    // Prints all of the finish jobs. TODO Jobs need Arrival Time and Finish Time  to show correct values.
    printf("Completed Jobs:\n");
    while (finished != NULL)
    {
        sum = sum + (finished->leaveTime - finished->arrivalTime);
        count++;
        printf("Job ID: %d Arrival Time: %d Finish Time: %d Turn Around Time: %d\n",
               finished->jobId, finished->arrivalTime, finished->leaveTime, (finished->leftTime - finished->arrivalTime));

        finished = finished->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all currently Hold Queue 1
    printf("Hold Queue 1: \n --------------------\n");
    while (hold_queue1 != NULL)
    {
        hold_queue1 = hold_queue1->head;
        printf("Job ID: %d Run Time: %d \n", hold_queue1->jobID, hold_queue1->burstTime);
        hold_queue1 = hold_queue1->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all currently on Hold Queue 2
    printf("Hold Queue 2: \n --------------------\n");
    while (hold_queue2 != NULL)
    {
        printf("Job ID: %d Run Time: %d \n", hold_queue2->processID, hold_queue2->burstTime);
        hold_queue2 = hold_queue2->next;
    }

    printf("---------------------------------------------------------------------------\n");
    // Prints all  currently Ready Queue; TODO Needs Time Accrued value
    printf("Ready Queue: \n ---------------------------------\n");
    if(ready_queue!=NULL&&ready_queue->next!=NULL){
        ready_queue=ready_queue->next;
    }
    while (ready_queue != NULL)
    {
        printf("Job ID: %d Run Time: %d Time Accrued: %d\n", ready_queue->processID, ready_queue->burstTime, ready_queue->accrued);
        ready_queue = ready_queue->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all  currently Wait Queue; TODO Needs Time Accrued value
    printf("Wait Queue: \n ---------------------------------\n");
    while (wait_queue != NULL)
    {
        printf("Job ID: %d Run Time: %d Time Accrued: %d\n", wait_queue->processID, wait_queue->burstTime, wait_queue->accrued);
        wait_queue = wait_queue->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all process on CPU; TODO needs Time Accrued and Time Left value; currently time left
    // is burstTime-Accrued this will only work if burstTime is updated while on CPU (decreases with time on CPU)
    printf("Running on CPU: \n---------------------------------\n");
    if(onCPU!=NULL){
        if(time_passed!=0){
    printf("Job ID: %d Time Accrued: %d Time Left: %d\n", onCPU->processID, onCPU->accrued+(time-time_passed), (onCPU->burstTime - (onCPU->accrued+(time-time_passed))));
        }else{
           printf("Job ID: %d Time Accrued: %d Time Left: %d\n", onCPU->processID, onCPU->accrued+(time-onCPU->arrival), (onCPU->burstTime - (onCPU->accrued+(time-onCPU->arrival)))); 
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

/* void displayJob(struct Job* j, int type){
    if(type == 1){
        printf("Type: SJF");
    }
    else if (type == 2){
        printf("Typle: FIFO");
    }
    else{
        printf("Invalid");
    }
} */
