#define SJF_Queue 1
#define FIFO_Queue 2

typedef struct Job{
    int jobId;
    int priority;
    int needMemory;
    int needDevice;
    //int holdDevice;
    //int requestDevice;
    int totalTime;
    //int leftTime;
    int arrivalTime;
    int leaveTime;
    Job* next;
}Job;

typedef struct Queue{
    Job* head;
    Job* tail;
    int queueType; // 1: SJF 2:FIFO
    int size;
}Queue;


//main.c functions
Job* newJob(Command* c);
void displayJob(Job* j, int type);
Queue* newQueue(int type);
void pushQueue(Queue* q, Job* j);
Job* popQueue(Queue* q);
int emptyQueue(Queue*q);
void displayJob(Job* j, int type);
