#include <stdio.h>
#include <stdlib.h>
#include "Job.h"
#include "command.h"
#include "System.h"

//creates a new job from a command
struct Job* newJob(struct Command* c){
    struct Job* newJob = (struct Job*)malloc(2+sizeof(struct Job));
    //does set job based on command
    if(newJob == NULL){
        return NULL;
    }
    newJob->next = NULL;
    newJob->jobId = c->jobId;
    newJob->priority = c->priority;
    newJob->burstTime = c->runTime;
    newJob->needMemory = c->memory;
    newJob->needDevice = c->devices;
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



void printQueue(struct Queue *queue)
{
    //this was mainly for debugging. it prints the current order of the queue passed in as a parameter
    printf("%s\n", "---------printing queue------");
    while (queue != NULL)
    {
        printf("JobNumber:%d with remaining %d\n", queue->head->jobId,queue->head->burstTime-queue->head->leftTime);
        queue->head = queue->head->next;
    }
    printf("%s\n", "----------------------------");
}