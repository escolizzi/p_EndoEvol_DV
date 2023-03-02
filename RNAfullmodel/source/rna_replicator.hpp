
#include <iostream>

#include "rna_fold.hpp"
#include "rna_string_cpp.hpp"

#ifndef RNAREPLICATOR
#define RNAREPLICATOR

class RNAreplicator{
  RNAstring rna_string;
  std::string end3;
  std::string end5;
  double kcat;
  double krec;
  unsigned int term;
  double kdis;

 public:
  inline RNAreplicator(const std::string &seq,RNAFold &rna_fold);
  inline RNAreplicator(std::string::size_type size,double (*genrand)(void),RNAFold &rna_fold);
  inline RNAreplicator(const RNAreplicator &rhs);
  inline RNAreplicator();
  inline const RNAreplicator &operator=(const RNAreplicator &rhs);
  ~RNAreplicator(){}

  /* Randomization */
  inline void randomizeReplicator(std::string::size_type size,double (*genrand)(void),RNAFold &rna_fold);

  /* Access to attributes */
  inline const std::string &getSequence() const;
  inline const std::string &getStructure() const;
  inline const std::string &getEnd3() const;
  inline const std::string &getEnd5() const;
  inline double getKcat() const;
  inline unsigned int getTerm() const;
  inline double getKdis() const;

  /* The score of base-pairs between the given end3 and this->end5. */
  inline unsigned int getPairScoreToEnd5(const std::string &a_end3) const;
  

  /* Replication */
  inline std::string getCmplSequence() const;

 private:
  /* A function used during construction */
  inline void init_replicator(RNAFold &rna_fold);
  /* This method returns the score of a base pair. */
  inline unsigned int if_base_pair(char base1,char base2) const;
  /* This method returns the sum score of base pairs between
     two string up to the 'length'. */
  inline unsigned int n_base_pair(const char *seq1,const char *seq2,unsigned int length) const;
  /* This method returns the maximum number of sum basepair
     score between open ends. We count GC as 3, AU as 2, GU as
     1. */
  unsigned int max_match(const char *seq1,const char *seq2) const;
};

RNAreplicator::RNAreplicator(const std::string &seq,RNAFold &rna_fold)
  : rna_string(seq,rna_fold),term(0),kdis(0.)
{
  if(!seq.empty())
    init_replicator(rna_fold);
  else
    std::cerr << "RNAreplicator::RNAreplicator() Try to make size=0 RNA string." << std::endl;
}

RNAreplicator::RNAreplicator(const RNAreplicator &rhs)
  : rna_string(rhs.rna_string), end3(rhs.end3), end5(rhs.end5), kcat(rhs.kcat), krec(rhs.krec), term(rhs.term), kdis(rhs.kdis)
{}

RNAreplicator::RNAreplicator(std::string::size_type size,double (*genrand)(void),RNAFold &rna_fold)
  : rna_string(size,genrand,rna_fold),term(0),kdis(0.)
{
  if(size>0)
    init_replicator(rna_fold);
  else
    std::cerr << "RNAreplicator::RNAreplicator() Try to make size=0 RNA string." << std::endl;
}

RNAreplicator::RNAreplicator()
  : rna_string(),end3(),end5(),kcat(0.),krec(0.),term(0),kdis(0)
{}

const RNAreplicator &RNAreplicator::operator=(const RNAreplicator &rhs)
{
  if(this!=&rhs){
    rna_string=rhs.rna_string;
    end3=rhs.end3;
    end5=rhs.end5;
    kcat=rhs.kcat;
    krec=rhs.krec;
    term=rhs.term;
    kdis=rhs.kdis;
  }
  return *this;
}

void RNAreplicator::randomizeReplicator(std::string::size_type size,double (*genrand)(void),RNAFold &rna_fold)
{
  rna_string.randomizeSequence(size,genrand,rna_fold);
  init_replicator(rna_fold);
}

const std::string &RNAreplicator::getSequence() const
{
  return rna_string.getSequence();
}

const std::string &RNAreplicator::getStructure() const
{
  return rna_string.getStructure();
}

const std::string &RNAreplicator::getEnd3() const
{
  return end3;
}
const std::string &RNAreplicator::getEnd5() const
{
  return end5;
}
double RNAreplicator::getKcat() const
{
  return kcat;
}
unsigned int RNAreplicator::getTerm() const
{
  return term;
}
double RNAreplicator::getKdis() const
{
  return kdis;
}

unsigned int RNAreplicator::getPairScoreToEnd5(const std::string &a_end3) const
{
  return RNAreplicator::max_match(a_end3.c_str(),end5.c_str());
}

std::string RNAreplicator::getCmplSequence() const
{
  return rna_string.getCmplSequence();
}

void RNAreplicator::init_replicator(RNAFold &rna_fold)
{
  /* 3'-end */
  std::string::size_type pos = rna_string.getStructure().find("(");
  end3.assign(rna_string.getSequence(),0,pos);

  /* 5'-end */
  pos = rna_string.getStructure().rfind(")");
  end5.assign(rna_string.getSequence(),pos+1,std::string::npos);
  reverse(end5.begin(),end5.end());

  /* Determin kcat */
  if(rna_fold.dist(rna_string.getStructure())==0)
    kcat = 1.;
  else
    kcat=0.;
}

unsigned int RNAreplicator::if_base_pair(char base1,char base2) const
{
  switch (base1) {
  case 'A': {
    switch(base2) {
    case 'A': return 0;
    case 'C': return 0;
    case 'G': return 0;
    case 'U': return 2;
    default : return 0;
    }
  }
  case 'C': {
    switch(base2) {
    case 'A': return 0;
    case 'C': return 0;
    case 'G': return 3;
    case 'U': return 0;
    default : return 0;
    }
  }
  case 'G': {
    switch(base2) {
    case 'A': return 0;
    case 'C': return 3;
    case 'G': return 0;
    case 'U': return 1;
    default : return 0;
    }
  }
  case 'U': {
    switch(base2) {
    case 'A': return 2;
    case 'C': return 0;
    case 'G': return 1;
    case 'U': return 0;
    default : return 0;
    }
  }
  default:
    return 0;
  }
}

unsigned int RNAreplicator::n_base_pair(const char *seq1,const char *seq2,unsigned int length) const
{
  int nmatch=0;
  for(unsigned int i=0;i<length;i++){
    nmatch+=RNAreplicator::if_base_pair(*(seq1+i),*(seq2+i));
  }
  return nmatch;
}

#endif // RNAREPLICATOR
