#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef COMMAND_H
#define COMMAND_H

struct Command{
    char type; //command type
    int time; // start/arrival time
    int jobId; //"A" command
    int memory; // "C" command "M"
    int devices; // "C" command "S" and "Q"/"L" command "D"
    int quantum;//"C" command "Q"
    int runTime; //"A" command R
    int priority;//"A" command P
};

int parseParameter(char* line, char* type, int* num, int start);
struct Command* parseCommand(char* line);

const char COMMAND_TYPE_C = 'C';
const char COMMAND_TYPE_A = 'A';
const char COMMAND_TYPE_Q = 'Q';
const char COMMAND_TYPE_L = 'L';
const char COMMAND_TYPE_D = 'D';

#endif