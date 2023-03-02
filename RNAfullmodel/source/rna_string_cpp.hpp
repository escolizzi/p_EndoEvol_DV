/*
  RNAstring 

  It checks if the sequence is an RNA and if the structure is
  an RNA structure.

  '<,>,==' is done by the string comparison.
  '=' is coping everything.
*/

#include <string>
#include <iostream>
#include <algorithm>

#include "rna_fold.hpp"

#ifndef RNASTRING
#define RNASTRING

class RNAstring{
 private:
  std::string sequence;
  std::string structure;
  double energy;
  
 public:
  /* This constructure sets the sequence. But not the
     structure. */
  inline RNAstring(const std::string &seq,RNAFold &rna_fold);
  /* The copy constructure is default */
  inline RNAstring(const RNAstring &rhs);
  /* This constructure creates random rna sequence of
     'size'. The constructure needs a function returns value
     in [0,1). */
  inline RNAstring(std::string::size_type size,double (*genrad)(void),RNAFold &rna_fold);
  /* Default constructure will create sequence with
     size=0. */
  inline RNAstring();
  virtual ~RNAstring(){}
  
  /* Sequence */
  /* Setting a new sequence will do structure="" and energy=0. */
  inline void setSequence(const std::string &seq,RNAFold &rna_fold);
  inline const std::string &getSequence() const;
  /* The method randomizes the sequence. The mothods takes as
     an argument a function pointer which returns a value in
     [0,1). */
  inline void randomizeSequence(std::string::size_type size,double (*genrand)(void),RNAFold &rna_fold);
  /* To get the complementary sequence of the original sequence */
  inline std::string getCmplSequence() const;

  /* Structure and energy */
  inline void setStructure(const std::string &str);
  inline const std::string &getStructure() const;

  inline void setEnergy(double ene);
  inline double getEnergy() const;

  inline const RNAstring &operator=(const RNAstring &rhs);

  /* Predicates are done only according to sequence */
  inline bool operator<(const RNAstring &rhs) const;
  inline bool operator>(const RNAstring &rhs) const;
  inline bool operator==(const RNAstring &rhs) const;

 private:
  inline bool isRNAstring(std::string seq) const;
  inline bool isRNAstructure(std::string str) const;
  inline char RNAi2c(int rna) const; /* RNA integer to RNA character */
  inline int RNAc2i(char rnachar);
  inline char RNAc2cc(char rna) const; /* RNA interger to complementary RNA character */
};

RNAstring::RNAstring(const std::string &seq, RNAFold &rna_fold)
  : sequence(seq), 
    structure(),
    energy(0.)
{
  if(sequence.empty()){
    std::cerr << "RNAstring::RNAstring() Try to create size=0 RNA sequence." << std::endl;
    return;
  }

  if( !RNAstring::isRNAstring(seq) ){
    std::cerr << "RNAstring::RNAstring() The sequence is not an RNA." << std::endl;
    std::exit(1);
  }

  /* Fold and set the energy */
  energy = rna_fold.fold(sequence,structure);
  if( !RNAstring::isRNAstructure(structure) ){
    std::cerr << "RNAstring::RNAstring() The structure is not an RNA structure." << std::endl;
    std::exit(1);
  }
}

RNAstring::RNAstring(const RNAstring &rhs)
  : sequence(rhs.sequence),
    structure(rhs.structure),
    energy(rhs.energy)
{}

RNAstring::RNAstring(std::string::size_type size,double (*genrand)(void),RNAFold &rna_fold)
  : sequence(), 
    structure(),
    energy(0.)
{
  if(size==0){
    std::cerr << "RNAstring::RNAstring() Try to create size=0 random RNA string." << std::endl;
    return;
  }

  if(sequence.capacity() < size)
    sequence.reserve(size);
  for(std::string::size_type i=0;i<size;i++){
    sequence.push_back(RNAstring::RNAi2c((int)(4*(*genrand)())));
  }

  /* Fold and set the energy */
  energy = rna_fold.fold(sequence,structure);
  if( !RNAstring::isRNAstructure(structure) ){
    std::cerr << "RNAstring::RNAstring() The structure is not an RNA structure." << std::endl;
    std::exit(1);
  }
}

RNAstring::RNAstring()
  : sequence(),
    structure(),
    energy(0.)
{}

void RNAstring::setSequence(const std::string &seq,RNAFold &rna_fold)
{
  if(RNAstring::isRNAstring(seq)){
    sequence = seq;
  }else{
    std::cerr << "RNAstring::setSequence() The given sequence is not an RNA." << std::endl;
  }

  /* Fold and set the energy */
  energy = rna_fold.fold(sequence,structure);
  if( !RNAstring::isRNAstructure(structure) ){
    std::cerr << "RNAstring::RNAstring() The structure is not an RNA structure." << std::endl;
    std::exit(1);
  }
}

const std::string &RNAstring::getSequence() const
{
  return sequence;
}

void RNAstring::randomizeSequence(std::string::size_type size,double (*genrand)(void),RNAFold &rna_fold)
{
  sequence.clear();
  if(sequence.capacity() < size)
    sequence.reserve(size);
  for(std::string::size_type i=0;i<size;i++){
    sequence.push_back(RNAstring::RNAi2c((int)(4*genrand())));
  }

  /* Fold and set the energy */
  energy = rna_fold.fold(sequence,structure);  
  if( !RNAstring::isRNAstructure(structure) ){
    std::cerr << "RNAstring::RNAstring() The structure is not an RNA structure." << std::endl;
    std::exit(1);
  }
}

std::string RNAstring::getCmplSequence() const
{
  std::string complementary;

  if(sequence.empty()){
    std::cerr << "RNAstring::getCmplSequence() Try to obtain the complementary sequence of null string" << std::endl;
    return complementary;
  }
  
  for(std::string::size_type i=0;i<sequence.size();i++){
    complementary.push_back(RNAstring::RNAc2cc(sequence[i])); 
  }

  /* convert the 3' and 5' order */
  reverse(complementary.begin(),complementary.end());

  return complementary;
}

void RNAstring::setStructure(const std::string &str)
{
  if(RNAstring::isRNAstructure(str)){
    structure = str;
  }else{
    std::cerr << "RNAstring::setStructure() The given structure is not an RNA structure." << std::endl;
  }
}

const std::string &RNAstring::getStructure() const
{
  if(structure.empty()){
    std::cerr << "RNAstring::getStructure() Try to get a structure which is null." << std::endl;
  }
  return structure;
}

void RNAstring::setEnergy(double ene)
{
  energy = ene;
}

double RNAstring::getEnergy() const
{
  return energy;
}

const RNAstring &RNAstring::operator=(const RNAstring &rhs)
{
  if(this!=&rhs){
    sequence = rhs.sequence;
    structure = rhs.structure;
    energy = rhs.energy;
  }
  return *this;
}

bool RNAstring::operator<(const RNAstring &rhs) const
{
  return (this->sequence < rhs.sequence);
}

bool RNAstring::operator>(const RNAstring &rhs) const
{
  return (this->sequence > rhs.sequence);
}

bool RNAstring::operator==(const RNAstring &rhs) const
{
  return (this->sequence == rhs.sequence);
}

/* char RNAi2c(int rnaint) 

   Parameter: give a RNA digit in 'rnaint' to change it to RNA
   character.
  
   Description: RNA ind to character.
   
   Return value: 'A' for 0, 'C' for 1, 'G' for 2, 'U' for '3',
   '\0' for error.
*/
char RNAstring::RNAi2c(int rna) const
{
  switch (rna) {
  case 0: 
    return ('A');
  case 1:
    return ('C');
  case 2:
    return ('G');
  case 3:
    return ('U');
  default:
    std::cerr << "RNArandom::RNAi2c() I got non RNA character. Be sure that genrand() returns a value in [0,1)." << std::endl;
    std::exit(1);
  }
  return 0;
}

/* 
   int RNAc2i(char rnachar)
   
   Parameter: give a RNA character (ACGU).
   
   Description: RNA character to RNA integer
   
   Return value: 0 for 'A', 1 for 'C', 2 for 'G', 3 for 'U',
   -1 for error..
*/
int RNAstring::RNAc2i(char rnachar)
{
  switch (rnachar) {
  case 'A':
    return 0;
  case 'C':
    return 1;
  case 'G':
    return 2;
  case 'U':
    return 3;
  default:
    perror("RNAc2i: I got a non RNA character");
    return -1;
  }
}

char RNAstring::RNAc2cc(char rna) const
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

/* Method returns true when size=0 */
bool RNAstring::isRNAstring(std::string seq) const 
{
  std::string::size_type size=seq.size();
  for(std::string::size_type i=0;i<size;i++){
    if(seq[i]!='A' && seq[i]!='U' && seq[i]!='G' && seq[i]!='C'){
      return false;
    }
  }
  return true;
}

/* Method returns true when size=0 */
bool RNAstring::isRNAstructure(std::string str) const
{
  std::string::size_type size=str.size();
  int match=0;
  for(std::string::size_type i=0;i<size;i++){
    switch(str[i])
      {
      case '.': break;
      case '(': match++; break;
      case ')': match--; break;
      default: return false;
      }
  }
  if(match==0){
    return true;
  }else{
    return false;
  }
}

#endif // RNASTRING
