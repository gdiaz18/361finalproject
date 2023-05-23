#include "Job.h"
#include "command.h"

//typedef struct System System;
#ifndef SYSTEM_H
#define SYSTEM_H

struct System{
    int time; //current time
    int totalMemory;
    int curMemory;
    int totalDevice; //availible = total - curr
    int curDevice;
    int timeQuantum;
    struct Queue* holdQueue1;
    struct Queue* holdQueue2;
    struct Queue* waitQueue;
    struct Queue* readyQueue;
    struct Queue* leaveQueue;
    struct Job* running;
    int startTime;
};

//initialize functions that will be used in system.c
struct System* newSystem(struct Command* c);
int nextEvent(struct System* s, struct Command* c);
int arriveJob(struct System* s, struct Job* j);
void scheduleQueue(struct System* s);
void moveOutHold(struct System* s);
void moveReadyToRunning(struct System* s);
void jobComplete(struct System* s);
void moveWaitToReady(struct System* s);
void moveRunningToReady(struct System* s);
void moveRunningToWait(struct System* s);
void requestDevice(struct System* s, struct Command* c, int numProcesses);
void releaseDevice(struct System* s, struct Command* c);
int bankers(struct System* s, struct Command* c, int numProcesses);
int printAtTime(struct System *s, int availableMem, int availableDevices);


#endif
