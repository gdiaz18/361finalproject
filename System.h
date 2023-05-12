#include "Job.h"

typedef struct System{
    int time; //current time
    int totalMemory;
    int curMemory;
    int totalDevice;
    int curDevice;
    int timeQuantum;
    Queue* holdQueue1;
    Queue* holdQueue2;
    Queue* waitQueue;
    Queue* readyQueue;
    Queue* leaveQueue;
    Job* running;
    int startTime;
}System;

//initialize functions that will be used in system.c
System* newSystem(Command* c);
int nextEvent(System* s, Command* c);
int arriveJob(System* s, Job* j);
void scheduleQueue(System* s);
void moveOutHold(System* s);
void moveReadyToRunning(System* s);
void jobComplete(System* s);
void moveWaitToReady(System* s);
void moveRunningToReady(System* s);
void moveRunningToWait(System* s);
void requestDevice(System* s, Command* c);
void releaseDevice(System* s, Command* c);
