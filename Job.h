#define SJF_Queue 1
#define FIFO_Queue 2

#include "command.h"

#ifndef JOB_H
#define JOB_H

//typedef struct Job Job;
//typedef struct Queue Queue;

struct Job{
    int jobId;
    int priority;
    int needMemory;
    int needDevice;
    int holdDevice; //allocation
    int requestDevice; //max
    int totalTime;
    int burstTime;
    int leftTime;
    int arrivalTime;
    int leaveTime;
    struct Job* next;
};

struct Queue{
    struct Job* head;
    struct Job* tail;
    int queueType; // 1: SJF 2:FIFO
    int size;
};


//main.c functions
struct Job* newJob(struct Command* c);
void displayJob(struct Job* j, int type);
struct Queue* newQueue(int type);
void pushQueue(struct Queue* q, struct Job* j);
struct Job* popQueue(struct Queue* q);
int emptyQueue(struct Queue* q);
void displayJob(struct Job* j, int type);

#endif /*JOB_H*/