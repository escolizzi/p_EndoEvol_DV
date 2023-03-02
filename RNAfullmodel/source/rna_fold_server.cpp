#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>

#include <iostream>
#include <sstream>

#include "rna_fold_server.hpp"

static const int R=0;
static const int W=1;

int RNAFoldServer::popen2(const char *command,int *fd_r, int *fd_w)
{
  int fd_p2c[2]; /* File descripter. Information flows from parent to child */
  int fd_c2p[2]; /* File descripter. ---------------------- child  to parent */
  int pid;
  
  /* Open create a pipe for IPC with process which will be
     invoked by 'command' */
  if(pipe(fd_p2c) < 0){
    std::perror("RNAFoldServer::popen2() cannot open() a pipe");
    std::exit(1);
  }
  if(pipe(fd_c2p) < 0){
    std::perror("RNAFoldServer::popen2() cannot open() a pipe");
    if(close(fd_p2c[R]) != 0)
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    if(close(fd_p2c[W] != 0))
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    std::exit(1);
    }

  /* Here we fork() */
  pid=fork();
  if (pid < 0){ 
    std::perror("RNAFoldServer::popen2() cannot fork()");
    if(close(fd_p2c[R]) != 0)
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    if(close(fd_p2c[W]) != 0)
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    if(close(fd_c2p[R]) != 0)
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    if(close(fd_c2p[W]) != 0)
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    return(-1);
  }
  else if(pid == 0){ /* This is the child */
    if(close(fd_p2c[W]) != 0) /* Close FD for the parent's writing to the child */
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    if(close(fd_c2p[R]) != 0) /* Close FD for the parent's reading from the child */
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");

    if(dup2(fd_p2c[R],0) == -1) /* Sets stdin to fd_p2c[R] */
      std::perror("RNAFoldServer::popen2() cannot dup2()");
    if(dup2(fd_c2p[W],1) == -1) /* Sets stdout to fd_c2p[W] */
      std::perror("RNAFoldServer::popen2() cannot dup2()");

    /* We don't use these anymore. But this dosen't close stdin stdout */
    if(close(fd_p2c[R]) != 0) 
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");
    if(close(fd_c2p[W]) != 0)
      std::perror("RNAFoldServer::popen2() cannot close() a file descripter");

    if(execlp("sh","sh","-c",command,NULL) < 0){ /* execlp() is dangerous due to
						    command search. */
      std::perror("RNAFoldServer::popen2() cannot execlp() command");
      std::exit(1);
    }
  }
  
  /* This is actually only for the parent */
  if(close(fd_p2c[R]) != 0) /* Close FD for the child's reading from the parent */
    std::perror("RNAFoldServer::popen2() cannot close() FD");
  if(close(fd_c2p[W]) != 0) /* Close FD for the child's writing to the parent */
    std::perror("RNAFoldServer::popen2() cannot close() FD.");
  
  (*fd_r) = fd_c2p[R]; /* One can read from child, stdout of executed process */
  (*fd_w) = fd_p2c[W]; /* One can write to child, stdin of executed process */
  
  return(pid);
}

RNAFoldServer::RNAFoldServer(const std::string &dist_method,const std::string &a_target_structure)
  : target_structure(a_target_structure)
{
  /* start RNAfold process. */
  pid_RNAfold = RNAFoldServer::popen2("RNAfold",&fd_RNAfold_r,&fd_RNAfold_w);
  if(pid_RNAfold == -1){
    std::perror("RNAFoldServer::RNAFoldServer() cannot popen2() RNAfold.");
    std::exit (1);
  }
  
  /* start RNAdistance process. */
  std::string distance_command="RNAdistance";
  distance_command = distance_command + " -Xp -D" + dist_method;
  pid_RNAdistance = RNAFoldServer::popen2(distance_command.c_str(),&fd_RNAdistance_r,&fd_RNAdistance_w);
  if(pid_RNAdistance == -1){
    std::perror("RNAFoldServer::RNAFoldServer() cannot popen2() RNAdistance.");
    std::exit (1);
  }

  /* start RNAeval process. */
  pid_RNAeval = RNAFoldServer::popen2("RNAeval",&fd_RNAeval_r,&fd_RNAeval_w);
  if(pid_RNAeval == -1){
    std::perror("RNAFoldServer::RNAFoldServer() cannot popen2() RNAeval.");
    std::exit (1);
  }

}

RNAFoldServer::~RNAFoldServer()
{
  if(kill(pid_RNAfold,SIGINT)==-1){
    perror("RNAFoldServer::~RNAFoldServer cannot kill RNAfold process.");
  }
  if(kill(pid_RNAdistance,SIGINT)==-1){
    perror("RNAFoldServer::~RNAFoldServer cannot kill RNAdistance process.");
  }
  if(kill(pid_RNAeval,SIGINT)==-1){
    perror("RNAFoldServer::~RNAFoldServer cannot kill RNAeval process.");
  }
}

double RNAFoldServer::fold(const std::string &seq,std::string &str)
{
  std::string::size_type size = seq.size();
  
  /* Do not forget to write '\n' at the end which tell RNAfold
     process */
  if(write(fd_RNAfold_w,seq.c_str(),size) == -1){
    perror("RNAFoldServer::fold() write error");
    exit(1);
  }
  if(write(fd_RNAfold_w,"\n",1) == -1){
    perror("RNAFoldServer::fold() write error");
    exit(1);
  }

  char buff[size+1];

  /* Discard the first line. Don't forget to take '\n' at the end */
  if(read(fd_RNAfold_r,buff,size+1) == -1){
    perror("RNAFoldServer::fold() write error");
    exit(1);
  }

  /* Take the second line which has a second structure */
  if(read(fd_RNAfold_r,buff,size+1) == -1){
    perror("RNAFoldServer::fold() write error");
    exit(1);
  }
  buff[size] = '\0';
  str = buff;

  /* Get the energy of the structure */
  char c;
  std::string energy_str;
  while(read(fd_RNAfold_r,&c,1),c != '\n'){
    if((isdigit(c) || c=='-') || c=='.'){
      energy_str.push_back(c);
    }
  }
  double energy;
  std::istringstream(energy_str) >> energy;
  return energy;
}

int RNAFoldServer::dist(const std::string &str) const
{
  if(target_structure.empty()){
    std::cerr << "RNAFoldServer::dist() The target structure is not defined." << std::endl;
    return 0;
  }

  /* Input the target structure */
  if(write(fd_RNAdistance_w,target_structure.c_str(),target_structure.size()) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }
  if(write(fd_RNAdistance_w,"\n",1) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }

  /* Input the argument structure */
  if(write(fd_RNAdistance_w,str.c_str(),str.size()) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }
  if(write(fd_RNAdistance_w,"\n",1) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }

  char c;
  std::string dist_str;
  while(read(fd_RNAdistance_r,&c,1),c != '\n'){
    if(isdigit(c)){
      dist_str.push_back(c);
    }
  }
  int dist;
  std::istringstream(dist_str) >> dist;
  return dist;
}


int RNAFoldServer::dist(const std::string &str1,const std::string &str2) const
{
  if(write(fd_RNAdistance_w,str1.c_str(),str1.size()) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }
  if(write(fd_RNAdistance_w,"\n",1) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }


  if(write(fd_RNAdistance_w,str2.c_str(),str2.size()) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }
  if(write(fd_RNAdistance_w,"\n",1) == -1){
    perror("RNAFoldServer::dist() write error");
    std::exit(1);
  }

  char c;
  std::string dist_str;
  while(read(fd_RNAdistance_r,&c,1),c != '\n'){
    if(isdigit(c)){
      dist_str.push_back(c);
    }
  }
  int dist;
  std::istringstream(dist_str) >> dist;
  return dist;
}

double RNAFoldServer::eval(const std::string &sequence,const std::string &structure) const
{
  std::string::size_type size = sequence.size();

  /* The length of sequence and structure must be the same */
  if(size != structure.size()){
    std::cerr << "RNAFoldServer::eval() The sequence and the structure have anequal length." << std::endl;
    std::exit(1);
  }

  /* Input the sequence */
  if(write(fd_RNAeval_w,sequence.c_str(),size) == -1){
    perror("RNAFoldServer::eval() write error");
    std::exit(1);
  }
  if(write(fd_RNAeval_w,"\n",1) == -1){
    perror("RNAFoldServer::eval() write error");
    std::exit(1);
  }

  /* Input the structure */
  if(write(fd_RNAeval_w,structure.c_str(),size) == -1){
    perror("RNAFoldServer::eval() write error");
    std::exit(1);
  }
  if(write(fd_RNAeval_w,"\n",1) == -1){
    perror("RNAFoldServer::eval() write error");
    std::exit(1);
  }


  /* Discard the first two lines. Don't forget to take '\n' at
     the end of the first line and ' ' in the second line
     after the structure. */
  char buff[2*size+2];
  if(read(fd_RNAeval_r,buff,2*size+2) == -1){
    perror("RNAFoldServer::fold() write error");
    exit(1);
  }

  char c;
  std::string dist_str;
  while(read(fd_RNAeval_r,&c,1),c != '\n'){
    if(isdigit(c) || c=='.' || c=='-'){
      dist_str.push_back(c);
    }
  }
  double energy;
  std::istringstream(dist_str) >> energy;
  return energy;
}
