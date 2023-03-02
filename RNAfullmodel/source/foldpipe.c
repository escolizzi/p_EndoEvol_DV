/* functions to fold RNA string via sub-process by using
   pipes. It is supposed that user can open only one RNAfold
   process. But, this can be midified easily. */

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>


#ifndef FOLDPIPE
#define FOLDPIPE

#define R 0 /* For file descripters. It represents "read" */
#define W 1 /* ----------------------------------- "write" */

static int fd_RNAfold_r; /* file descriptor to read */
static int fd_RNAfold_w; /* file descriptor to write */
static int pid_RNAfold; /* process id of RNAfold */

static int fd_RNAdistance_r;
static int fd_RNAdistance_w;
static int pid_RNAdistance;

/* popen2(char *command,int fd_r, int fd_w)
   Parameter: Put a string you want to execute as 'command'.
   Put an address to the file descripter which you want to 
   have set to stdout of the 'command' process in 'fd_r'.
   In the same way, put an address in 'fd_w' for stdout.

   Description: popen2 takes two file descripters and execulte
   'command', then it puts a pipe for reading and for writing to
   taken file descripters. 

   Return value: If succesful, process id of child process 
   (executed process). If failur, -1.
*/

static int popen2(char *command,int *fd_r, int *fd_w)
{
  int fd_p2c[2]; /* File descripter. Information flows from parent to child */
  int fd_c2p[2]; /* File descripter. ---------------------- child  to parent */
  int pid;

  /* Open create a pipe for IPC with process which will be invoked by 'command' */
  if(pipe(fd_p2c) < 0){
    perror("popen2: cannot open() a pipe");
    exit(1);
  }
  if(pipe(fd_c2p) < 0){
    perror("popen2: cannot open() a pipe");
    if(close(fd_p2c[R]) != 0)
      perror("popen2: cannot close() a file descripter");
    if(close(fd_p2c[W] != 0))
      perror("popen2: cannot close() a file descripter");
    exit(1);
    }

  /* Here we fork() */
  pid=fork();
  if (pid < 0){ 
    perror("popen2: cannot fork()");
    if(close(fd_p2c[R]) != 0)
      perror("popen2: cannot close() a file descripter");
    if(close(fd_p2c[W]) != 0)
      perror("popen2: cannot close() a file descripter");
    if(close(fd_c2p[R]) != 0)
      perror("popen2: cannot close() a file descripter");
    if(close(fd_c2p[W]) != 0)
      perror("popen2: cannot close() a file descripter");
    return(-1);
  }
  else if(pid == 0){ /* This is the child */
    if(close(fd_p2c[W]) != 0) /* Close FD for the parent's writing to the child */
      perror("popen2: cannot close() a file descripter");
    if(close(fd_c2p[R]) != 0) /* Close FD for the parent's reading from the child */
      perror("popen2: cannot close() a file descripter");

    if(dup2(fd_p2c[R],0) == -1) /* Sets stdin to fd_p2c[R] */
      perror("popen2: cannot dup2()");
    if(dup2(fd_c2p[W],1) == -1) /* Sets stdout to fd_c2p[W] */
      perror("popen2: cannot dup2()");

    /* We don't use these anymore. But this dosen't close stdin stdout */
    if(close(fd_p2c[R]) != 0) 
      perror("popen2: cannot close() a file descripter");
    if(close(fd_c2p[W]) != 0)
      perror("popen2: cannot close() a file descripter");

    if(execlp("sh","sh","-c",command,NULL) < 0){ /* execlp() is dangerous due to
						    command search. */
      perror("popen2: cannot execlp() command");
      exit(1);
    }
  }
  
  /* This is actually only for the parent */
  if(close(fd_p2c[R]) != 0) /* Close FD for the child's reading from the parent */
    perror("popen2: cannot close() FD");
  if(close(fd_c2p[W]) != 0) /* Close FD for the child's writing to the parent */
    perror("popen2: cannot close() FD.");
  
  (*fd_r) = fd_c2p[R]; /* One can read from child, stdout of executed process */
  (*fd_w) = fd_p2c[W]; /* One can write to child, stdin of executed process */
  
  return(pid);
}


/* 
   The function will start up two processes, namely RNAfold and
   RNAdistance.

   Parameters: 'structure' should be the target structure to
   which the RNADistancePipe() will calculate the distance.

 */
void RNAOpen(char *structure)
{
  int rna_len;

  pid_RNAfold = popen2("RNAfold",&fd_RNAfold_r,&fd_RNAfold_w);
  if(pid_RNAfold == -1){
    perror("RNAfold: cannot popen2() RNAfold.");
    exit (1);
  }

  pid_RNAdistance = popen2("RNAdistance -Xf -DC",&fd_RNAdistance_r,&fd_RNAdistance_w);
  if(pid_RNAdistance == -1){
    perror("RNAfold: cannot popen2() RNAdistance");
    exit (1);
  }

  /* get the length of RNA seq */
  rna_len = strlen(structure);

  /* put \n chara at the end */
  structure[rna_len] = '\n';

  /* Do not forget to write rna_len+1 byte. The last one byte is
     '\n' which tell RNAdistance process the end of input line. */
  if(write(fd_RNAdistance_w,structure,rna_len+1) != rna_len+1){
    perror("RNAOpen(): write error");
    exit(1);
  }
 
  /* put back \0 chara at the end */
  structure[rna_len] = '\0';
}


/* return 0 if success. Otherwise -1. */
int RNAClose(void)
{
  if(kill(pid_RNAfold,SIGINT)==-1){
    /* cannot kill all children */
    return (-1);
  }
  if(kill(pid_RNAdistance,SIGINT)==-1){
    /* cannot kill all children */
    return (-1);
  }
  return(0);
}

/* int RNAFoldPipe

   Parameter: Give a sequence you want to fold in 'sequence[]'.
   Give a string array in 'structure[]'. It will get it to feed
   the second structure of the 'sequence[]'. 'sequence[]' must
   contain '\0' at the end of sequence. 

   Descrpition: It calculate minimam free energy 2nd structure of
   RNA sequence. Then, it put its branket representation in given
   char pointer. It uses RNAfold command via pipes.

   Return value: If failure, -1. If success, 0.
*/
float RNAFoldPipe(char *sequence,char *structure)
{
  char c;
  int rna_len;
  char buff[100];
  int current=0;

  /* get the length of RNA seq */
  rna_len = strlen(sequence);

  /* put \n chara at the end */
  sequence[rna_len] = '\n';

  /* Do not forget to write rna_len+1 byte. The last one byte is '\n' which tell
     RNAfold process */
  if(write(fd_RNAfold_w,sequence,rna_len+1) != rna_len+1){
    perror("RNAFoldPipe:write error");
    exit(1);
  }

  /* Discard the first line. Don't forget to take '\n' at the end */
  if(read(fd_RNAfold_r,structure,rna_len+1) != rna_len+1){
    perror("RNAFoldPipe:read error");
    exit(1);
  }

  /* Take the second line which has a second structure */
  if(read(fd_RNAfold_r,structure,rna_len) != rna_len){
       perror("RNAFoldPipe:read error");
    exit(1);
  }

  /* Put '\0' at the end */
  sequence[rna_len] = '\0';
  structure[rna_len]='\0';

  /* Get the energy of the structure */
  while(read(fd_RNAfold_r,&c,1),c != '\n'){
    if((isdigit(c) || c=='-') || c=='.'){
      buff[current] = c;
      current++;
    }
  }
  buff[current] = '\0';
  return  (float) atof(buff);
}

/*
  The function will calculate the distance between the target
  structure and the given argument structure. The target
  structure will be set when RNAOpen() is called. The measure of
  distance is by string alighment with the use of coarse grained
  structure. 

  Parameters: 'structure' should take a branket notation of
  structure. 


*/
int RNADistancePipe(char *structure)
{
  char c;
  int rna_len;
  char buff[100];
  int current=0;

  /* get the length of RNA seq */
  rna_len = strlen(structure);

  /* put \n chara at the end */
  structure[rna_len] = '\n';

  /* Do not forget to write rna_len+1 byte. The last one byte is '\n' which tell
     RNAfold process */
  if(write(fd_RNAdistance_w,structure,rna_len+1) != rna_len+1){
    perror("RNAFoldPipe:write error");
    exit(1);
  }

  /* Put '\0' at the end */
  structure[rna_len]='\0';

  /* Get the energy of the structure */
  while(read(fd_RNAdistance_r,&c,1),c != '\n'){
    if(isdigit(c)){
      buff[current] = c;
      current++;
    }
  }
  buff[current] = '\0';
  return  atoi(buff);
}

#endif /* FOLDPIPE */
