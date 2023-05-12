#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Device.h"
#include "Job.h"
#include "System.h"
#include "command.h"

//Global Variables

//Device devices[MAX_DEVICES];
//Process process_table[MAX_PROCESSES];

int time_slice = 4;
int current_time = 0;
int available_memory = 256;
int num_processes = 0;

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

	System *system;


	//reading input files function
	char file[500];
	
	char file_name[100]; //ask about string error

	//opening file
	FILE* ptr = fopen(file_name, "r");

	if(NULL == ptr){
		printf("File cannot be opened");
	}

	//reading through each line
	while (fgets(file, sizeof(file), ptr) != NULL){
		//call parsing functions and then push to queue
		Command* command = parseCommand(file);

		//System Config
		switch (command->type){
			case 'COMMAND_TYPE_C': {
				Command* info = command;

				//need to incorporate queues?

				system->time = info->time;
				system->totalMemory = info->memory;
				system->curMemory = system->totalMemory;
				system->totalDevice = info->devices;
				system->curDevice = system->totalDevice;
				system->timeQuantum = info->quantum;

				break;
			}
			//Job arrival
			case 'COMMAND_TYPE_A': {
				Command* info = command;

				Job* job = {info->jobId, info->priority, info->memory, info->devices, info->time, info->runTime,  0};
				//need to incorporate jobs?? FIFO? SJF?
				if(job->needMemory > system->totalMemory || job->needDevice > system->totalDevice){
					printf("job is rejected, resource is not enough");
				}
				if(system->curMemory >= job->needMemory){
					printf("adding job to ready queue");
					//push job to readyqueue
					pushQueue(system->readyQueue, job);
					//system->curMemory = system->totalMemory - readyQueue->totalMemory
				}
				else{
					if(job->priority == 1){
						//push job into hold queue 1 SJF 
						pushQueue(system->holdQueue1, job);
					}
					else{
						//push job into hold queue 2 FIFO 
						pushQueue(system->holdQueue2, job);
					}
				}

				break;
			}
			//Request for Jobs
			case 'COMMAND_TYPE_Q': {
				Command* info = command;

				Job job = {info->jobId, info->priority, info->memory, info->devices, info->time, info->runTime,  0};
				//use if statement to compare number of devices - number of devices used greater than or equal to used devices
				if((system->totalDevice - system->curDevice) >= system->curDevice){
					
				}
				//push that job into ready queue; else if # of devices = to need, running job pushed into waiting queue

				break;

			}

			case 'COMMAND_TYPE_L': {

				break;
			}

			case 'COMMAND_TYPE_D': {

				break;

			}

		}
	}

	
}



