typedef struct Process{
	int pid;
	int arrival_time;
	int burst_time;
	int memory_required;
	int time_left;
	int priority;
	int devicce_required;
}Process;

const int MAX_PROCESSES = 100;
