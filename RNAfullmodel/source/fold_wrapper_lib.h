#ifndef FOLDWRAPPERLIB_H
#define FOLDWRAPPERLIB_H

struct RNAFoldLibCash;


struct RNAFoldLibCash *FoldLib_1_Init(void);
double FoldLib_fold(struct RNAFoldLibCash *obj,const char *sequence,char *structure);
unsigned int FoldLib_size(struct RNAFoldLibCash *onj);
void FoldLib_clear(struct RNAFoldLibCash *obj);
void FoldLib_insert(struct RNAFoldLibCash *obj,const char *sequence,const char *structure,double energy);
void FoldLib_setTargetStructure(struct RNAFoldLibCash *obj,const char *target_structure);
int FoldLib_dist(struct RNAFoldLibCash *obj,const char *structure);
double FoldLib_eval(struct RNAFoldLibCash *obj,const char *sequence,const char *structure);
#endif //FOLDWRAPPERLIB_H
