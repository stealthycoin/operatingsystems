#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

//pretend booleans
#define TRUE 1
#define FALSE 0

//color codes
#define KBLD  "\x1B[1m"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

extern char **get_line(void);

/**
 * Handle death of a child process
 * At the moment it doesn't really 
 * do anything, ideally it was going to 
 * clean up the terminal after the 
 * output of the child process is printed.
 */

void sigchld_handler(int sig) {
}

/**
 * Forks a process 
 */
void execute(char** args, int background, char* infile, char* outfile, char* errfile, int pipe) {
  pid_t pid;
  int status;
  int fd[2]; //file descriptors for pipes
  pipe(fd);
  if ((pid = fork()) < 0) {
    printf("Failed to fork child process");
    exit(1);
  }
  else if (pid == 0) {
    close(fd[1]);
    if (infile != NULL) {
      //redirect stdin
      int in;
      in = open(infile, O_RDONLY);
      dup2(in,0);
      close(in);
    }
    if (outfile != NULL) {
      //redirect stout
      int out;
      out = open(outfile, 
		 O_WRONLY | O_TRUNC | O_CREAT, 
		 S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
      dup2(out,1);
      close(out);
    }
    if (errfile != NULL) {
      int err;
      err = open(errfile,
		 O_WRONLY | O_TRUNC | O_CREAT,
		 S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
      dup2(err,2);
      close(err);
    }

    if (background == TRUE) {
      printf("\n");
    }

    if (execvp(*args, args) < 0) {
      exit(EXIT_FAILURE);
    }
  }
  else {
    //parent process block if it is not set to end in the background
    if (background == FALSE) {
      while (wait(&status) != pid);
    }
  }
}


/**
 * Main function, waits for input from the user and orchestrates system calls
 * based on the format if the input.
 */
int main(int argc, char *argv[]) {
  //this sets the sigchld handler, which I currently do not use
  //Ill leave this in case I start using it again.
  struct sigaction sa;
  memset(&sa,0,sizeof(sa));
  sa.sa_handler = sigchld_handler;
  sigaction(SIGCHLD, &sa, NULL);
  
  int i;
  char **args; 
  char path[255];
  while(1) {

    //make a pretty prompt
    getcwd(path,255);
    printf("%s%s%s%s ->%s ",KBLD,KCYN,path,KRED,KNRM);
    args = get_line();
    if(!args || args[0] == NULL) continue; //empty command skip to next command

    int error = FALSE;
    int background = FALSE;
    int pipe = FALSE;
    char* infile = NULL;
    char* outfile = NULL;
    char* errfile = NULL;

    //process < and > and &
    for (i = 0 ; args[i] != NULL ; i++) {
      if (strncmp(args[i], "<", 1) == 0) {
	++i;
	if (args[i] == NULL) {
	  printf("Parse error missing infile\n");
	  error = TRUE;
	}
	else {
	  infile = args[i];
	}
      }
      else if (strncmp(args[i], ">", 1) == 0) {
	++i;
	if (args[i] == NULL) {
	  printf("Parse error missing outfile\n");
	  error = TRUE;
	}
	else {
	  outfile = args[i];
	}
      }
      else if (strncmp(args[i], "2>", 2) == 0) {
	++i;
	if (args[i] == NULL) {
	  printf("Parse error missing err file\n");
	  error = TRUE;
	}
	else {
	  errfile = args[i];
	}
      }
      else if (strncmp(args[i], "|", 1) ==0) {
	//everything from 0 to i-1 is command one
	//from i+1 to the end is the  command two
	args[i] = NULL;
	pipe = TRUE;
      }
    }
    //check if it should be executed in the background, this is handled poorly
    if (i >= 2 && strncmp(args[i-1], "&", 1) == 0) {
      background = TRUE;
      args[i-1] = NULL;
    }

    //If the command is exit, exit the shell and display a pretty message
    if (strncmp(args[0], "exit", 4) == 0) {
      printf("%sShell Exiting%s\n",KGRN,KNRM);
      exit(EXIT_SUCCESS);
    }
    //else execute the command as long as there was not a parse error
    else if (error == FALSE) {
      execute(args, background, infile, outfile, errfile, pipe);
    }
  }
  return 0;
}
