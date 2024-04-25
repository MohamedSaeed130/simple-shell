#include "lab1.h"




int main()
{
	signal(SIGCHLD, func);
	setup_environment();
	Shell();
	return 0;
}

void func (int signum)
{
	int status=0; 
	while(waitpid((pid_t)(-1),0,WNOHANG)>0);
	int fd=open("hhh_file.txt",O_CREAT|O_WRONLY);
	write(fd,"Child terminated!!",18);
}

void Shell(void)
{
	char *command[50]={'\0'}; // array to take the input command
	
        while(1)
        {
			parse_input(command);

			/* To select the type of input command such as Executable command like ls,ps,pwd,... or 
			Builtin command like cd,echo and export */

			switch (Input_type)
			{
			case Executable_command:
				execute_command(command);
				break;
			case Builtin_command:
				execute_shell_bultin(command);
				break;
			default:
				break;
			}

		}
}
/**
 * @Breif:function to parse the input command into tokens
 * @param: take double pointer to array of charchters by reference
 * @return: return nothing
*/
void parse_input(char ** Command_array)
{

	char input_command[100]={'\0'};
	char *Current_dir=NULL;
	while(1)
	{
		Input_type=Executable_command;
		GREEN(); 									 /* To color the output by green */
		printf("%s@Sell:~",UserName);  					 /* Print on the screen user name of the system*/
		BLUE();										 /* To color the output by blue */
		if(strcmp(getcwd(Current_dir,100),getenv("HOME")))
		{
			char size=strlen(getenv("HOME"));
			char *str=getcwd(Current_dir,100);
			printf("%s$>>",str+size); /* Print on the screen current working directory of the system*/
		}
		else{printf("$>>");}
		   
		RESET(); 									 /* To color the output by default colors*/
 		fgets(input_command,100,stdin);				 /* Take input from user*/
		if(input_command[0]=='\n' || input_command[0]==' ') /* continue if user press enter or space*/
			continue;
		input_command[strlen(input_command)-1]='\0'; 		/*initialize this array with NULLs*/
		int i=0;
		/*Divide the input command based on the space */
		Command_array[0]=strtok(input_command," "); 

		while(Command_array[i] != NULL)
		{
			Command_array[++i]=strtok(NULL," ");
		}

		if(!strcmp(Command_array[0],"exit")) /* Exit from the Shell*/
        	exit(0);
		else{}
		/*To check if the input command is Executable command or Builtin command */
		if((!strcmp(Command_array[0],"export")) || (!strcmp(Command_array[0],"echo")) || (!strcmp(Command_array[0],"cd")))
		{
			Input_type=Builtin_command;
		}
		else{}

		break;
	}
}
/**
 * @Breif:function to execute the input command if it is Executable command.
 * @param: take double pointer to array of charchters by reference
 * @return: return nothing
*/

void execute_command(char **Command_array)
{
	int status=0;
	int i=0;
	char Background_flag=0;
	while((Command_array[i])!=NULL)
	{
		if(!strcmp(Command_array[i],"&"))
			{Background_flag=1;break;} /* If the command needs to run in the background */
		else
			{Background_flag=0;}
		i++;
	}

	int pid=fork();
	if(pid>0) /* In parent process*/
	{
		/* If the command needs to run in the background the parent process should not wait for its child */
		if(1==Background_flag) 
			{}
		else
			{waitpid(pid,&status,0);}
	}
	else if (0==pid) /* In child process*/
	{
		execvp(Command_array[0],Command_array); /* Execute the command */
	    exit(0);// if user input is not a vaild command 
	}
	else /* If fork failed to spawn a child*/
		exit(-1);
}

void execute_shell_bultin(char *Command_array[])
{
	if(!strcmp(Command_array[0],"cd"))
	{
		
		if((Command_array[1]==NULL) || (!strcmp(Command_array[1],"~")))
			chdir(home_dir);
		else
			chdir(Command_array[1]);
	}
	else if(!strcmp(Command_array[0],"export"))
	{	
		int i=0;
		char export_param[10]={'\0'};
		strcpy(export_param,Command_array[1]);
		char *tok1=NULL;
		char *tok2=NULL;
		tok1=strtok(export_param,"=");
		tok2=strtok(NULL,"=");
		if(tok2==NULL)
			setenv(tok1,"",1);
		else
			setenv(tok1,tok2,1);
		 
	}
	else if(!strcmp(Command_array[0],"echo"))
	{
		char echo_param[100]={'\0'};
		strcpy(echo_param,Command_array[1]);
		char *token_1=NULL;
		char *token_2=NULL;
		token_1=strtok(echo_param,"$");
		token_2=strtok(NULL,"$");
		if(token_2 == NULL)
		{
			if(echo_param[0]=='$')
			{
				if(getenv(token_1))
					printf("%s\n",getenv(token_1));
				else
					printf("\n");
			}
			else
				printf("%s\n",token_1);	
		}
		else 
		{
			printf("%s%s\n",token_1,getenv(token_2));
		}
	}
	else
	{
		exit(0);
	}
}

void setup_environment(void)
{
	home_dir=getenv("HOME");
	UserName=getenv("USER");// get the user name
	chdir(home_dir);
}

void BLUE()
{
  printf("\033[1;34m");
}

void GREEN()
{
  printf("\033[0;32m");
}

void RESET () {
  printf("\033[0m");
}