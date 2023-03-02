/*
  setAllPopZero();
  for() insert();
  deleteZeroPopGenotype();
  communicatePobserver();
*/

#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

#include "genotype.hpp"
#include "p-observer.hpp"

#ifndef GOBSERVER
#define GOBSERVER
class Gobserver {
 
  typedef std::map<std::string,Genotype> tree_type;
  tree_type tree;

  unsigned int undetermined_pop;
  
 public:
  Gobserver():undetermined_pop(0){}
  ~Gobserver(){}
  void insert(const std::string &sequence,const std::string &structure,const std::string &end3,const std::string &end5,const std::string &csequence,const std::string &cstructure,const std::string &cend3,const std::string &cend5, const double &kcat);
  /* Method returns the population of the given sequence. If
     sequence="", then it returns the population of
     genotype which lacks the information of structure. */
  unsigned int getPop(const std::string &sequence="") const;
  /* Delete the node (of the tree) with zero population. Call
     this method after inserting all genotypes you want to
     insert. */
  void deleteZeroPopGenotype();
  /* To set all population size to zero. This method is called
     to prepare for inserting a new set of genotypes. */
  void setAllPopZero();
  /* This method makes the size of tree zero. */
  void clearAll();
  /* This method will insert all the genotypes g-observer
     knows into p-observer. Call this method after (1)
     inserting all genotypes you want and (2) deleting all
     genotypes in g-observer with population zero. */
  void communicatePobserver(Pobserver &pobserver);
  void dump();

 private:
  void insert(const std::string &sequence,const std::string &structure,const std::string &end3,const std::string &end5,const std::string &c_sequence,const std::string &c_structure,const std::string &c_end3,const std::string &c_end5);
  inline char RNAc2cc(char rna) const; /* RNA interger to complementary RNA character */
  inline void getCmplSeq(const std::string &seq,std::string &cseq) const;
  /* Return true if the first argument is the plus string (if 1st < 2nd ). */
  inline bool if1stIsPlusString(const std::string &seq1,const std::string &seq2) const;
};

char Gobserver::RNAc2cc(char rna) const
{
  switch (rna) {
  case 'A':
    return 'U';
  case 'C':
    return 'G';
  case 'G':
    return 'C';
  case 'U':
    return 'A';
  default:
    std::cerr << "RNAcc2c: I got a non RNA character" << std::endl;
    std::exit(1);
  }
  return 0;
}

void Gobserver::getCmplSeq(const std::string &seq,std::string &cseq) const
{
  if(!cseq.empty())
    cseq.clear();
  std::string::const_reverse_iterator end_iter = seq.rend();
  for(std::string::const_reverse_iterator iter = seq.rbegin();iter!=end_iter;iter++){
    cseq.push_back(Gobserver::RNAc2cc(*iter)); 
  }
}


bool Gobserver::if1stIsPlusString(const std::string &seq1,const std::string &seq2) const
{
  if(seq1<seq2)
    return true;
  else
    return false;
}
#endif // GOBSERVER
