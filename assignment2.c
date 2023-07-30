#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/stat.h>//mkdir header file
#include<sys/types.h>//mkdir header file
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<dirent.h>
#include <fcntl.h> //write and read files

//functions declared
void print_dir();
void input_from_user();
void welcome();
int contains_input;
int contains_output;

//global variables
int MAX_NUM_OF_CMD=250;

void input_from_user(char string_input[]){
	printf("\nj_shell> ");
	fgets(string_input,100,stdin);
}

void welcome(){
	//system("clear");  //clearing the console screen included in stdlib.h
	printf(" 		/-----------------------------\\ \n");
	printf("		\\                             |\n");
	printf("		|     Welcome to Randhir-sh!  |\n");
	printf("		/                             /\n");
	printf(" 		\\-----------------------------/\n");
}

void print_dir(){
	char Directory[1024];
	getcwd(Directory, sizeof(Directory));
	printf("You are here:> %s\n", Directory);
}

int contains_pipe(char* input_args, char** pipedArgs){
	int i;
	char* input_args1[MAX_NUM_OF_CMD];
	input_args1[0]=strsep(&input_args,"\n");
	for (int i=0;i<MAX_NUM_OF_CMD;i++){
		pipedArgs[i]=strsep(&input_args1[0],"|");
		if (pipedArgs[i]==NULL) break;
	}
	if (pipedArgs[1]==NULL){
		return 0;
	}
	return 1;
}

void parse_cmd(char* input_args, char** parsedArgs){
	int i;
	char* input_args1[MAX_NUM_OF_CMD];
	input_args1[0]=strsep(&input_args,"\n");
	for (i = 0; i < MAX_NUM_OF_CMD; i++) {
		parsedArgs[i] = strsep(&input_args1[0], " ");
		if (parsedArgs[i] == NULL)
			break;//whole string is traversed
		if (strlen(parsedArgs[i]) == 0)
			i--;//If extra spaces then the counter should
			    //be decreased to oerwrite the whitespaces
	}
}

void piped_exec(char* input_args, char** parsed, char** parsedpipe){
	// parsing correctly
	parse_cmd(parsedpipe[0],parsed);
	char* exec1[2];
	char* exec2[2];
	char temp1[250]="./";
	char temp2[250]="./";
	strcat(temp1,parsed[0]);
	exec1[0]=temp1;
	exec1[1]=NULL;
	parse_cmd(parsedpipe[1],parsed);
	strcat(temp2,parsed[0]);
	exec2[0]=temp2;
	exec2[1]=NULL;

	//piping begins!
	int pipefd[2];
	pid_t p1, p2;

	if (pipe(pipefd) < 0) {
		printf("\nPipe could not be initialized");
		return;
	}
	p1 = fork();
	if (p1 < 0) {
		printf("\nCould not fork");
		return;
	}
	if (p1 == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(exec1[0], exec1) < 0) {
			printf("\n[Could not execute program1]\n");
			exit(0);
		}
	} else {
		// Parent executing
		p2 = fork();

		if (p2 < 0) {
			printf("\nCould not fork");
			return;
		}

		if (p2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(exec2[0], exec2) < 0) {
				printf("\n[Could not execute program2]\n");
				exit(0);
			}
		} else {
			// parent executing, waiting for two children
			wait(NULL);
			wait(NULL);
		}
	}
}

void parseIO(char* args1,char* input, char*out, char*program){
	contains_input=0;
	contains_output=0;
	char  args3[100];
	char* args2;
	args2=strsep(&args1,"\n");
	strcpy(args3,args2);
	char args[100]="./";
	strcat(args,args3);
	char temp[100]="";
	int i;
	char mode='\0';
	for (i=0;i<100;i++){
		if (args[i]=='\0'){
			if (strcmp(temp,"")==0)
				break;
			if (mode=='\0'){
				strcpy(program,temp);
			}
			else if (mode=='>'){strcpy(out,temp);}
			else  {strcpy(input,temp);}
			strcpy(temp,"");
			break;
		}
		if (args[i]=='<'){
			if (mode=='\0'){strcpy(program,temp);}
			else if(mode=='>'){strcpy(out,temp);}
			strcpy(temp,"");
			mode ='<';
			contains_input=1;

		}
		if (args[i]=='>'){
			if (mode=='\0'){strcpy(program,temp);}
			else if(mode=='<'){strcpy(input,temp);}
			strcpy(temp,"");
			mode ='>';
			contains_output=1;
		}
		if (args[i]!=' ' && args[i]!='>' && args[i]!='<'){
			char c=args[i];
			strncat(temp,&c,1);
		}
	}
}

void exe_exec(char* temp,char** args){
		char input[250];
		char out[250];
		char program[250];
		parseIO(temp,input,out,program);
		pid_t pid=fork();
		if (pid<0){
			printf("forking failed....\n");
		}
	 else if (pid==0){
		char* exec[2];
		exec[0]=program;
		exec[1]=NULL;
		int f1,f2;
		if (contains_input){
			f1=open(input,O_RDONLY);
			dup2(f1,0);
			close(f1);
		}
		if (contains_output){
					if((f2 = open(out, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0){
		      	perror("open error");
		    	}
					dup2(f2,1);
					close(f2);
		}
		if (execvp(exec[0],exec)<0){
			printf("program cannot be executed ....");
		}
		exit(0);
	}
	else{
		wait(NULL);
	}
}

int execute(char* input_args, char** parsedArgs, char** pipedArgs){
	char temp[250];
	char temp1[250];
	strcpy(temp,input_args);
	strcpy(temp1,input_args);
	int exec_mode=0;
	exec_mode=contains_pipe(input_args, pipedArgs);
	if (exec_mode){
		piped_exec(temp, parsedArgs, pipedArgs);
	}
	else {
		parse_cmd(temp,parsedArgs);
		if (strcmp(parsedArgs[0],"exit")==0){
			printf("\n______________Closing the J-shell_____________\n\n");
			exit(0);
		}
		else if(strcmp(parsedArgs[0],"cd")==0) {
			int success = chdir(parsedArgs[1]);
			if (success!=0){
				printf("[No such directory exists!]");
			}
			else print_dir();
		}
		else if(strcmp(parsedArgs[0],"pwd")==0){
			print_dir();
		}
		else if (strcmp(parsedArgs[0],"mkdir")==0){
			int success=mkdir(parsedArgs[1],0777);
			if (success!=0){
				printf("[Directory already exists!]\n");
			}
			else {
				printf("[Directory created successfully!]\n");
				print_dir();}
		}
		else if (strcmp(parsedArgs[0],"rmdir")==0){
			int success=rmdir(parsedArgs[1]);
			if (success!=0){
				printf("[No such directory exists!]\n");
			}
			else printf("[Directory deleted successfully!]\n");
		}
		else{
			exe_exec(temp1,parsedArgs);
		}
	}
	return 0;
}

int main(){
	char* parsedArgs[MAX_NUM_OF_CMD];//Args to be executed normally
	char* pipedArgs[MAX_NUM_OF_CMD];//Args to be pipelined

	//WELCOME
	welcome();
	int running=1;

	while(running){
		//input_from_user
		char input_args[250];
		input_from_user(input_args);
		if (strcmp(input_args,"\n")==0){
			continue;
		}
		//execute the input_args
		execute(input_args,parsedArgs,pipedArgs);
	}
	return 0;
}
