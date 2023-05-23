#define SJF_Queue 1
#define FIFO_Queue 2

#include "command.h"

#ifndef JOB_H
#define JOB_H


struct Job{
    int jobId;
    int priority;
    int needMemory;
    int needDevice; 
    int holdDevice; //allocation
    int requestDevice; //max
    int totalTime;
    int burstTime;
    int leftTime; //when job actually left/finished
    int arrivalTime;
    int leaveTime; //when it is going to leave/finish
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
void printQueue(struct Queue *queue);

#endif /*JOB_H*/