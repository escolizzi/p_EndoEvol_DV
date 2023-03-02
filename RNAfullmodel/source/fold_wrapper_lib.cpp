#include <string>
#include <cstring>
#include "rna_fold_server_cash.hpp"
#include "rna_fold_lib_cash.hpp"

namespace FoldWrapperLib {
  static RNAFoldLibCash fold_lib = RNAFoldLibCash();
}

extern "C" RNAFoldLibCash *FoldLib_1_Init(void)
{
  return &FoldWrapperLib::fold_lib;
}


extern "C" double FoldLib_fold(RNAFoldLibCash *obj,char *sequence,char *structure)
{
  return obj->fold(sequence,structure);
}

extern "C" unsigned int FoldLib_size(RNAFoldLibCash *obj)
{
  return (unsigned int)obj->size();
}

extern "C" void FoldLib_clear(RNAFoldLibCash *obj)
{
  obj->clear();
}

extern "C" void FoldLib_insert(RNAFoldLibCash *obj,const char *sequence,const char *structure,double energy)
{
  obj->insert(sequence,structure,energy);
}

extern "C" void FoldLib_setTargetStructure(RNAFoldLibCash *obj,const char *target_structure)
{
  obj->setTargetStructure(target_structure);
}

extern "C" int FoldLib_dist(RNAFoldLibCash *obj,const char *structure)
{
  return obj->dist(structure);
}

extern "C" double FoldLib_eval(RNAFoldLibCash *obj,const char *sequence,const char *structure)
{
  return obj->eval(sequence,structure);
}
