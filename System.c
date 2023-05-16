#include "System.h"
#include <stdio.h>
#include <stdlib.h>
#include "Job.h"
#include "command.h"



//initialize system configuration
//check if next event is internal or external
//parse the input when the job arrives
//schedule a job into different queue
//move the job from the hold queue to the ready queue
//move from ready queue to running queue to make job complete

//arive_job
//job_complete
//movejob to ready
//move job to running




struct System* newSystem(struct Command* c){
    //crete a new system
    struct System* system = (struct System*)malloc(sizeof(struct System));

    //initialize
    system->time = 0;
    system ->totalMemory = c->memory;
    system->curMemory = system -> totalMemory;
    system -> totalDevice = c->devices;
    system-> curDevice = system->totalDevice;
    system -> timeQuantum = c->quantum;
    system -> holdQueue1 = newQueue(1); //SJF
    system -> holdQueue2 = newQueue(2); //FIFO
    system->waitQueue = newQueue(-1);
    system-> readyQueue = newQueue(-1);
    system->leaveQueue= newQueue(-1);
    system->running =0;
    system->startTime = 0;

    return system;
};

//this function im not sure about cause its suppose to return a number
int nextEvent(struct System* s, struct Command* c){
    //check if job in hold queue
    if((s->holdQueue1->size != 0) || (s->holdQueue2->size != 0)){
        arriveJob(s,c); //confused cause 2nd param should be a job
        return 0; //success
        //reutrn EVENT_ARRIVE_JOB
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
    //want execution time is that the same as leaveTime - arrivalTime?
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
    //move jobs from ready to running state
    if(s->running == NULL && (s->readyQueue->size != 0)){
        s->running = popQueue(s->readyQueue);
        s->startTime = s->time;
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

void requestDevice(struct System* s, struct Command* c){
    //check is system has availible devices
    if(s->curDevice > 0){
        s-> curDevice--;
    }
    else{
        //if no availible devices push job to wait queue
        pushQueue(s->waitQueue, c->jobId); //this might wrong
    }
};

void releaseDevice(struct System* s, struct Command* c){
    //release device and icnrement availible device count
    s->curDevice++;
};
