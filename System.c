#include "System.h"
#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include"command.h"



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




System* newSystem(Command* c){

};

int nextEvent(System* s, Command* c){

};

int arriveJob(System* s, Job* j){

};

void scheduleQueue(System* s){

};

void moveOutHold(System* s){

};

void moveReadyToRunning(System* s){

};

void jobComplete(System* s){

};

void moveWaitToReady(System* s){

};

void moveRunningToReady(System* s){

};

void moveRunningToWait(System* s){

};

void requestDevice(System* s, Command* c){

};

void releaseDevice(System* s, Command* c){

};
