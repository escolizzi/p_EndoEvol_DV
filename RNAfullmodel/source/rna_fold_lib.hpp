/*
  Class RNAFoldLib

  Error check to the input RNA sequences or RNA 2ndary
  structures are omitted. By default, the comparison of the
  secondary structure is done by the coarse grained structure
  representation (expanded shapiro).

  (May be) very important difference from
  RNAFoldServer. dist() does not return the string editing
  (alignment) distance, but simply the return value as
  follows: If the structure is different, returns 1; otherwise returns 0. 
*/

#include <string>
#include <iostream>

#include <cstdlib>
#include <cstring>

extern "C" {
#include <fold.h>
#include <RNAstruct.h>
#include <dist_vars.h>
#include <treedist.h>
}


#include "rna_fold.hpp"

#ifndef RNAFOLDLIB
#define RNAFOLDLIB

class RNAFoldLib : public RNAFold {

  char *target_expand_shapiro;

 public:
  /*  If 'a_target_structure' is given, one can use 'fold'
     method with only one argument of structure: The distance
     between the argument structure and the given target
     structure is calculated in the subsequent call of 'dist'
     method. 'a_target_structure' is dot-bracket notation. For
     distance methods, choose one of the "fhwcFHWCP". See man
     page of RNAdistance. */
  inline RNAFoldLib(const std::string &a_target_structure="");
  inline ~RNAFoldLib();
  
  inline void setTargetStructure(const std::string &a_target_structure);
  
  inline double fold(const std::string &seq,std::string &str);
  inline double fold(char *seq,char *str);
  inline int dist(const std::string &str) const;
  inline int dist(char *structure) const;
  inline int dist(const std::string &str1,const std::string &str2) const;
  inline int dist(char *str1,char *str2) const;
  inline double eval(const std::string &sequence,const std::string &structure) const;
  inline double eval(char *sequence,char *structure) const;
  inline void full_str2coarse_str(std::string &course_structure,const std::string &structure) const;
};


RNAFoldLib::RNAFoldLib(const std::string &a_target_structure)
  : target_expand_shapiro(NULL)
{
  if(!a_target_structure.empty()){
    char *b2c;
    b2c = ::b2C(a_target_structure.c_str());
    target_expand_shapiro = expand_Shapiro(b2c);
    free(b2c);
  }
}

RNAFoldLib::~RNAFoldLib()
{
  free(target_expand_shapiro);
}
void RNAFoldLib::setTargetStructure(const std::string &a_target_structure)
{
  if(!a_target_structure.empty()){
    char *b2c;
    b2c = ::b2C(a_target_structure.c_str());
    target_expand_shapiro = expand_Shapiro(b2c);
    free(b2c);
  }
}


double RNAFoldLib::fold(const std::string &seq,std::string &str)
{
  char sequence[seq.size()+1];
  char structure[seq.size()+1];
  strcpy(sequence,seq.c_str());
  double energy = ::fold(sequence,structure);
  str = structure;
  return energy;
}

double RNAFoldLib::fold(char *seq,char *str)
{
  return (double)::fold(seq,str);
}

int RNAFoldLib::dist(const std::string &str) const
{
  if(target_expand_shapiro==NULL){
    std::cerr << "RNAFoldLib::dist() The target structure is not defined." << std::endl;
    return 0;
  }

  char structure[str.size()+1];
  std::strcpy(structure,str.c_str());
  char *b2c = ::b2C(structure);
  char *expand_shapiro = ::expand_Shapiro(b2c);
  int dist = std::strcmp(target_expand_shapiro,expand_shapiro);
  std::free(b2c);
  std::free(expand_shapiro);
  if(dist)
    return 1;
  else
    return 0;
}

int RNAFoldLib::dist(char *structure) const
{
  if(target_expand_shapiro==NULL){
    std::cerr << "RNAFoldLib::dist() The target structure is not defined." << std::endl;
    return 0;
  }


  char *b2c = ::b2C(structure);
  char *expand_shapiro = ::expand_Shapiro(b2c);
  int dist = std::strcmp(target_expand_shapiro,expand_shapiro);
  std::free(b2c);
  std::free(expand_shapiro);
  if(dist)
    return 1;
  else
    return 0;
}

int RNAFoldLib::dist(const std::string &str1,const std::string &str2) const
{
  char structure1[str1.size()+1];
  std::strcpy(structure1,str1.c_str());
  char * b2c_1 = ::b2C(structure1);
  char *expand_shapiro_1 = ::expand_Shapiro(b2c_1);

  char structure2[str2.size()+1];
  std::strcpy(structure2,str2.c_str());
  char * b2c_2 = ::b2C(structure2);
  char *expand_shapiro_2 = ::expand_Shapiro(b2c_2);

  int dist = std::strcmp(expand_shapiro_1,expand_shapiro_2);

  free(b2c_1);
  free(expand_shapiro_1);

  free(b2c_2);
  free(expand_shapiro_2);

  if(dist)
    return 1;
  else
    return 0;
}

double RNAFoldLib::eval(const std::string &sequence,const std::string &structure) const
{
  char seq[sequence.size()+1];
  char str[structure.size()+1];
  std::strcpy(seq,sequence.c_str());
  std::strcpy(str,structure.c_str());
  return (double)::energy_of_structure(seq,str,0);
}

double RNAFoldLib::eval(char *sequence,char *structure) const
{
  return (double)::energy_of_structure(sequence,structure,0);
}

void RNAFoldLib::full_str2coarse_str(std::string &course_structure,const std::string &structure) const
{
  char str[structure.size()+1];
  std::strcpy(str,structure.c_str());

  char *b2c = ::b2C(str);
  char *expand_shapiro = ::expand_Shapiro(b2c);
  course_structure = expand_shapiro;
  std::free(b2c);
  std::free(expand_shapiro);

}

#endif // RNAFOLDLIB
