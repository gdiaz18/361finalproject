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


#endif