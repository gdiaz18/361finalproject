**To Compile**: gcc -Wall -g -c Job.c main.c command.c System.c 

**To Link**: gcc -Wall -o main.exe command.o main.o System.o Job.o

**To run**: ./main.exe

**Comment in line 142 and comment out line 180 in main.c to show that printing output function works!

**When switching input files if user wanted, replace first parameter of fopen on line 52 with new file name.

Current Limitations:
    Tried to resolve segmentation fault/stackdump error that group members are experiencing through cygwin
    and clang compilers. Despite trying suggestions from TA and looking at information from other sources online, we could not fix our issue causing our program to stop running towards the end of case "A" considering there are no more warnings when compiling that could point to a possible problem
    ****Also in report*
