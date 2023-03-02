#ifndef FOLDWRAPPER_H
#define FOLDWRAPPER_H

struct RNAFoldServerCash;


struct RNAFoldServerCash *FoldServer_1_Init(void);
double FoldServer_fold(struct RNAFoldServerCash *obj,const char *sequence,char *structure);
unsigned int FoldServer_size(struct RNAFoldServerCash *onj);
void FoldServer_clear(struct RNAFoldServerCash *obj);
void FoldServer_insert(struct RNAFoldServerCash *obj,const char *sequence,const char *structure,double energy);
void FoldServer_setTargetStructure(struct RNAFoldServerCash *obj,const char *target_structure);
int FoldServer_dist(struct RNAFoldServerCash *obj,const char *structure);
double FoldServer_eval(struct RNAFoldServerCash *obj,const char *sequence,const char *structure);
#endif //FOLDWRAPPER_H
