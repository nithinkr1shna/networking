#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define OUTPUT_SIZE 9999
char output[OUTPUT_SIZE];
char header_and_data[OUTPUT_SIZE];

char* run_script(char *path){
    
  //char output[OUTPUT_SIZE]; 
    int numbytes;
    int link[2];
    pid_t pid, status =0 ;
    char cmd[250];
    //printf("%s\n", path);
    //printf("INSIDE RUN_SCRIPT\n");

    if(pipe(link) == -1)
      printf("error\n");
    //printf("after pipe\n");
    
    if((pid = fork()) == -1)
      printf("error in fork\n");

    if(pid == 0){
   	    printf("inside fork\n");
   	    dup2(link[1], STDOUT_FILENO);
   	    close(link[0]);
   	    close(link[1]);
        //sprintf(cmd, "%s%s","python ", path);
        //system(cmd);
	    if(execlp("sh","sh","-c", path, (char*)0) == -1)
               perror("execl");


    }else{
       
        close(link[1]);
    	numbytes = read(link[0], output, sizeof output);
      //close(link[0]);
      printf("Output: (%.*s)\n", numbytes, output);
      wait(NULL);
        
     
   }
    //printf("%s\n", output);
    return output;

}
