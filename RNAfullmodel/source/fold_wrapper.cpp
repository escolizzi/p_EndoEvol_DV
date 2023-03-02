#include <string>
#include <cstring>
#include "rna_fold_server_cash.hpp"
#include "rna_fold_lib_cash.hpp"

namespace FoldWrapper {
  static RNAFoldServerCash fold_1 = RNAFoldServerCash();
}

extern "C" RNAFoldServerCash *FoldServer_1_Init(void)
{
  return &FoldWrapper::fold_1;
}


extern "C" double FoldServer_fold(RNAFoldServerCash *obj,const char *sequence,char *structure)
{
  std::string str(structure);

  double energy;
  energy = obj->fold(sequence,str);
  
  strcpy(structure,str.c_str());

  return energy;
}

extern "C" unsigned int FoldServer_size(RNAFoldServerCash *obj)
{
  return (unsigned int)obj->size();
}

extern "C" void FoldServer_clear(RNAFoldServerCash *obj)
{
  obj->clear();
}

extern "C" void FoldServer_insert(RNAFoldServerCash *obj,const char *sequence,const char *structure,double energy)
{
  obj->insert(sequence,structure,energy);
}

extern "C" void FoldServer_setTargetStructure(RNAFoldServerCash *obj,const char *target_structure)
{
  obj->setTargetStructure(target_structure);
}

extern "C" int FoldServer_dist(RNAFoldServerCash *obj,const char *structure)
{
  return obj->dist(structure);
}

extern "C" double FoldServer_eval(RNAFoldServerCash *obj,const char *sequence,const char *structure)
{
  return obj->eval(sequence,structure);
}
