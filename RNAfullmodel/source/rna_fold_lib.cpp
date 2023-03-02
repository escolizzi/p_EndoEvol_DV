#include <iostream>
#include <string>

#include <cstdlib>
#include <cstring>

extern "C" {
#include <fold.h>
#include <RNAstruct.h>
#include <dist_vars.h>
#include <treedist.h>
}

#include "rna_fold_lib.hpp"

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
  free(b2c);
  free(expand_shapiro);
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
  free(b2c);
  free(expand_shapiro);
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
  return (double)::energy_of_struct(seq,str);
}

double RNAFoldLib::eval(char *sequence,char *structure) const
{
  return (double)::energy_of_struct(sequence,structure);
}
