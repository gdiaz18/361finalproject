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
int printAtTime(int used_devices,int used_memory,int time,int time_passed, int memory, int devices, struct Queue *hold_queue1, struct Queue *hold_queue2, struct Queue *ready_queue, struct Queue *wait_queue, struct Job *finished, struct Queue *onCPU);

#endif /*JOB_H*/