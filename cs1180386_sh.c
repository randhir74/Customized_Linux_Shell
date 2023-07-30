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
int main(){
  char* exec[2];
  exec[0]="./assignment2";
  exec[1]=NULL;
  pid_t pid=fork();
  if (pid<0){
    printf("forking failed....\n");
  }
 else if (pid==0){
    if (execvp(exec[0],exec)<0){
      printf("program cannot be executed ....");
    }
    exit(0);
  }
  else{
    wait(NULL);
  }
  printf("\n[This is a parent! I lost my child xD]\n");

}
