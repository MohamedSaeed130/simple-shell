#ifndef LAB1_H
#define LAB1_H

/***** Includes ******/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

/***** Macros ******/
#define Builtin_command    0
#define Executable_command 1


/***** Global Variables ******/
char Input_type=Executable_command;
char *home_dir=NULL;
char *UserName=NULL;

/***** Functions Prototyping ******/
void Shell(void);
void parse_input(char **Command_array);
void execute_command(char **Command_array);
void execute_shell_bultin( char *Command_array[]);
void setup_environment(void);
void func (int signum);
void BLUE  (void);
void GREEN (void);
void RESET (void);




















#endif