#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "command.h"

int parseParameter(char* line, char* type, int* num, int start) {
    int i = start;

    //skipping over whitespace
    while (line[i] == ' ') {
        i++;
    }

    //if newline or end of string is reached, jump out
    if (line[i] == '\0' || line[i] == '\n') {
        return -1;
    }

    //sets type to current character
    *type = line[i];

    //skips over following white space
    i += 2;

    //storing parse int value
    *num = atoi(line + i);

    //increments index while you are not on white space before reaching end or new line
    while (line[i] != ' ' && line[i] != '\0' && line[i] != '\n') {
        i++;
    }
    return i;

}

Command* parseCommand(char* line) {
    //allocates memory for new command struct
    Command* cmd = (Command*) malloc(sizeof(Command));


    if (cmd == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }

    //type to the first character in the string
    cmd->type = line[0];

    //parses through line, saving amount of time it takes to move through
    cmd->time = atoi(line + 2);

    //starting at whitespace after # assigned to first command
    int i = 3;


    char param_type;
    int param_value;
    while (i != -1 && line[i] != '\n' && line[i] != '\0') {
        //starting at 3rd postition in string to look for additional params
        i = parseParameter(line, &param_type, &param_value, i);
        switch (param_type) {
            case 'M':
                cmd->memory = param_value;
                break;
            case 'S':
                cmd->devices = param_value;
                break;
            case 'Q':
                cmd->quantum = param_value;
                break;
            case 'J':
                cmd->jobId = param_value;
                break;
            case 'D':
                cmd->devices = param_value;
                break;
            case 'P':
                cmd->priority = param_value;
                break;
            case 'R':
                cmd->runTime = param_value;
                break;
            default:
                fprintf(stderr, "Error: invalid parameter type %c\n", param_type);
                exit(1);
        }
    }

    return cmd;
}



int main(){

    return 0;
}






