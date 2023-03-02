
#include "fold_wrapper_lib.h"

#ifndef ANNOTMOL
#define ANNOTMOL
int MaxMatch(char *, char *);

/* It returns the maximum possible basepair score that the given
   sequence can make */
int maxbasepair(char* sequence);

/* Molecule annotation and replication */
inline void AnnotMol(TYPE2 *);
void AnnotMolCash(TYPE2 *);
void AnnotMolCashNoMutation(TYPE2 *child,TYPE2 *parent);

void AnnotPlane(TYPE2 **);
double MyFold(char*,char*); /* Use cashing */
double Ptype2kcat(char *);

void openEnd(TYPE2 *);
void Get3End(char *end3,char* seq,char *str);
void AssoDissRate(char *,char *,double *,double *);

int ColorByGenotype(char *);
int ColorByFunction(TYPE2*);
int ColorByPhenotype(TYPE2 *);

/*function used for phen det*/
char det_phen(char *seq,char *cseq,double kcat, char *end5, char *end3, double ckcat, char *cend5, char *cend3);
int min_hd_chd(char *seq1,char *cseq1,char *seq2);
int MaxBasePairScore(char *end);
#endif // ANNOTMOL
