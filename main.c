#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Device.h"
#include "Job.h"
#include "System.h"

//#ifdef COMMAND_H
//#define COMMAND_H
#include "command.h"
//#endif 

//testing branch

//Global Variables

//Device devices[MAX_DEVICES];
//Process process_table[MAX_PROCESSES];

int time_slice = 4;
int current_time = 0;
//int available_memory = 256;
int used_memory = 0;
int num_processes = 0;

// const char COMMAND_TYPE_C = 'C';
// const char COMMAND_TYPE_A = 'A';
// const char COMMAND_TYPE_Q = 'Q';
// const char COMMAND_TYPE_L = 'L';
// const char COMMAND_TYPE_D = 'D';

//sorting hold queue based on SJF algorithm
/* bool sjf_compare(Process p1, Process p2){
	return p1.burst_time < p2.burst_time;
} */

/* bool fifo_compare(Process p1, Process p2){
	return p1.arrival_time < p2.arrival_time;
} */

//adding process to hold queue
/* void add_to_hold_queue(Process p, string algorithm){
	if(available_memory < p.memory_required){
		hold_queue.push_back(p);
		if(algorithm == "sjf"){
			std::sort(hold_queue.begin(), hold_queue.end(), sjf_compare);
		}
		else if (algorithm == "fifo"){
			std::sort(hold_queue.begin(), hold_queue.end(), fifo_compare);
		}
		else{
			available_memory -= p.memory_required;
			p.time_left = p.burst_time;
			ready_queue.push(p);
		}
	} */



 int main(){
	//struct Devices tmp;
	struct System *system;

	//reading input files function
	char *file;
	
	//char file_name[100]; ask about string error

	//opening file
	FILE* ptr = fopen("i0.txt", "r");

	if(NULL == ptr){
		printf("File cannot be opened");
	}

	//reading through each line
	while (fgets(file, sizeof(file), ptr) != NULL){

		//call parsing functions and then push to queue
		struct Command* command = parseCommand(file);

		switch (command->type){
			//System Config
			case 'C': {
				struct Command* info = command;
				system = newSystem(info);

				//need to incorporate queues
				emptyQueue(system->holdQueue1);
				emptyQueue(system->holdQueue2);
				emptyQueue(system->readyQueue);
				emptyQueue(system->waitQueue);

				printf("-----Configuring----\n");

				system->time = info->time;
				system->totalMemory = info->memory;
				system->curMemory = system->totalMemory;
				system->totalDevice = info->devices;
				system->curDevice = system->totalDevice;
				system->timeQuantum = info->quantum;

				printf("Made system\n");

				//break;
			}
			//Job arrival
			case 'A': {
				struct Command* info = command;

				struct Job* job = newJob(info);
				//need to incorporate jobs?? FIFO? SJF?
				if(job->needMemory > system->totalMemory || job->needDevice > system->totalDevice){
					printf("job is rejected, resource is not enough\n");
				}
				if(system->curMemory >= job->needMemory){
					printf("adding job to ready queue\n");

					//push job to readyqueue
					pushQueue(system->readyQueue, job);

					used_memory = used_memory + job->needMemory;
					printf("Used memory: %d\n", used_memory);

					system->curMemory = system->totalMemory - job->needMemory;

					//need to check for internel event time??
				}
				else{
					if(job->priority == 1){
						//push job into hold queue 1 SJF 
						printf("Adding %d to Hold Queue 1\n", job->jobId);
						pushQueue(system->holdQueue1, job);

						printf("Done Adding!\n");
					}
					else{
						//push job into hold queue 2 FIFO 
						printf("Adding %d to Hold Queue 2\n", job->jobId);
						printf("Memory needed: %d, Available: %d,\n", job->needMemory, system->totalMemory - used_memory);
						pushQueue(system->holdQueue2, job);

						printf("Done adding!\n");
					}
				}

				//break;
			}
			//Request for Jobs
			case 'Q': {
				struct Command* info = command;

				struct Job* job = newJob(info);
				//use if statement to compare number of devices - number of devices used greater than or equal to used devices
				if((system->totalDevice - system->curDevice) >= system->curDevice){
					
				}
				//push that job into ready queue; else if # of devices = to need, running job pushed into waiting queue

				//break;

			}

			case 'L': {

				//break;
			}

			case 'D': {

				//break;

			}

			default : 
			//printf("broken line");
			return 0;

		}
	}
	printf("outside while loop");
	return 0;
}



