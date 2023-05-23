#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Job.h"
#include "System.h"


#include "command.h"


//Global Variables

int MAX_FILE_SIZE = 500;

int current_time = 0;
int available_memory = 256;
int used_memory = 0;
int num_processes = 0;
int used_devices = 0;

int internalEvent(struct System* s, int quantum, int time_passed){
    //checks if ready queue is null
    if (s->readyQueue == NULL){
        return 9999999;
    }
    else{
        if (s->readyQueue->head->leftTime + quantum <= s->readyQueue->head->burstTime){
            return time_passed + quantum;
        }
        else{
            return time_passed + (s->readyQueue->head->burstTime - s->readyQueue->head->leftTime);
        }
    }
}

 int main(){
	//Allocating memory for system and queues
	struct System* system = (struct System*)malloc(sizeof(struct System));

	system->holdQueue1 = (struct Queue*)malloc(sizeof(struct Queue));
	system->holdQueue2 = (struct Queue*)malloc(sizeof(struct Queue));
	system->readyQueue = (struct Queue*)malloc(sizeof(struct Queue));
	system->waitQueue = (struct Queue*)malloc(sizeof(struct Queue));
	system->leaveQueue = (struct Queue*)malloc(sizeof(struct Queue));

	//reading input files function
	char *file = (char*)malloc(sizeof(char) * MAX_FILE_SIZE);
	

	//opening file
	FILE* ptr = fopen("i0.txt", "r");

	//if file cannot be opened, exit
	if(ptr == NULL){
		printf("File cannot be opened");
		exit(1);
	}

	//reading through each line
	while (fgets(file, sizeof(file), ptr) != NULL){
		//call parsing functions and then push to queue
		struct Command* command = parseCommand(file);
		system = newSystem(command);

		switch (command->type){
			//System Config
			case 'C': {
				command = parseCommand(file);

				printf("-----Configuring----\n");

				system->time = command->time;
				printf("Time: %d\n", system->time);

				system->totalMemory = command->memory;
				printf("Memory: %d\n", system->totalMemory);

				system->curMemory = system->totalMemory;
				printf("Curr Memory: %d\n", system->curMemory);

				system->totalDevice = command->devices;
				printf("Total Device: %d\n", system->totalDevice);

				system->curDevice = system->totalDevice;
				printf("Curr Device: %d\n", system->curDevice);

				system->timeQuantum = command->quantum;
				printf("Quantum: %d\n", system->timeQuantum);

				current_time += system->time;

				//need to call interalEvent?

				printf("Made system\n");
				
			}
			//Job arrival
			case 'A': {
				command = parseCommand(file);

				struct Job* job = newJob(command);
				//Checking if job needs more memory or devices that are available
				if(job->needMemory > system->totalMemory || job->needDevice > system->totalDevice){
					printf("job is rejected, resource is not enough\n");
				}
				//if there is enough available memory
				if(system->curMemory >= job->needMemory){
					printf("adding job to ready queue\n");

					//push job to readyqueue
					pushQueue(system->readyQueue, job);

					//Adding to used memory based on how much job needed
					used_memory = used_memory + job->needMemory;

					//taking away memory needed by job from available memory
					available_memory = system->totalMemory - job->needMemory;

					printf("Used memory: %d\n", used_memory);
				}
				else{
					if(job->priority == 1){
						//push job into hold queue 1 SJF 
						printf("Adding %d to Hold Queue 1\n", job->jobId);
						pushQueue(system->holdQueue1, job);

						printf("Done Adding SJF!\n");
					}
					else{
						//push job into hold queue 2 FIFO 
						printf("Adding %d to Hold Queue 2\n", job->jobId);
						printf("Memory needed: %d, Available: %d,\n", job->needMemory, system->totalMemory - used_memory);
						pushQueue(system->holdQueue2, job);

						printf("Done adding FIFO!\n");
					}
				}

				//To show that function works when uncommented despite the fact is should not be called here
				//printAtTime(system, available_memory, system->totalDevice);

				//need to call interalEvent?

			}
			//Request for Jobs
			case 'Q': {
				command = parseCommand(file);


				struct Job* job = newJob(command);
				//compare number of devices & number of devices to push that job into ready queue; else if # of devices = to need, running job pushed into waiting queue
				if(job->jobId == system->running->jobId && (system->totalDevice + system->curDevice) <= system->running->needDevice){
					//Going to request device based on bankers
					requestDevice(system, command, num_processes);

					//Adding to used devices based on how many devices job needs
					used_devices += job->needDevice;

				}

			}
			//Release job
			case 'L': {
				command = parseCommand(file);

				struct Job* job = newJob(command);
				if (job->jobId == system->running->jobId && (system->totalDevice + system->curDevice) <= system->running->holdDevice){
					//Going to release devices
					releaseDevice(system, command);

					//Taking back devices job needed from amount used
					used_devices -= job->needDevice;
				}

			}

			case 'D': {
				printAtTime(system, available_memory, used_devices);				

			}

			default : 
			printf("broken line");
			return 0;

		 }
		free(command);

	}

	printf("Closing file");
	fclose(ptr);
	free(system->holdQueue1);
    free(system->holdQueue2);
    free(system->readyQueue);
    free(system->waitQueue);
    free(system->leaveQueue);
    free(system);
	return 0;
}



