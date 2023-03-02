/*
  Class RNAFoldServer

  Error check to the input RNA sequences or RNA 2ndary
  structures are omitted.
*/

#include <string>

#include "rna_fold.hpp"

#ifndef RNAFOLDSERVER
#define RNAFOLDSERVER

class RNAFoldServer : public RNAFold {
  int fd_RNAfold_r; /* file descriptor to read */
  int fd_RNAfold_w; /* file descriptor to write */
  int pid_RNAfold; /* process id of RNAfold */
  
  int fd_RNAdistance_r;
  int fd_RNAdistance_w;
  int pid_RNAdistance;

  int fd_RNAeval_r;
  int fd_RNAeval_w;
  int pid_RNAeval;

  std::string target_structure;

  int popen2(const char *command,int *fd_r, int *fd_w);

 public:
  /*  If 'a_target_structure' is given, one can use 'fold'
     method with only one argument of structure: The distance
     between the argument structure and the given target
     structure is calculated in the subsequent call of 'dist'
     method. 'a_target_structure' is dot-bracket notation. For
     distance methods, choose one of the "fhwcFHWCP". See man
     page of RNAdistance. */
  RNAFoldServer(const std::string &dist_method="C",const std::string &a_target_structure="");
  ~RNAFoldServer();
  
  inline void setTargetStructure(const std::string &a_target_structure);
  inline const std::string &getTargetStructure() const;
  
  double fold(const std::string &seq,std::string &str);
  int dist(const std::string &str) const;
  int dist(const std::string &str1,const std::string &str2) const;
  double eval(const std::string &sequence,const std::string &structure) const;
  
};

void RNAFoldServer::setTargetStructure(const std::string &a_target_structure)
{
  target_structure = a_target_structure;
}

const std::string &RNAFoldServer::getTargetStructure() const
{
  return target_structure;
}

#endif // RNAFOLDSERVER
