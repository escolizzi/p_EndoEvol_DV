#include <fold.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <RNAstruct.h>
#include <dist_vars.h>
#include <treedist.h>

#include "constants.h"
#include "para.h"
#include "cash2.h"
#include "rna_string.h"
#include "annotmol.h"
#include "foldcash.h"
#include "phenotree.h"

#include "fold_wrapper_lib.h"

extern int ncalls_myfold;
extern int nsucc_foldcash;
extern int nrow,ncol;

/* Static function declaration */
static int nbasepair(char*,char*,int);
static int ifbasepair(char,char);

/***************************************
 * Molecule annotation and replication *
 ***************************************/
/* You give a TYPE2 pointer variable, given that "seq" has a
   right sequence (not like empty flag 'N'). The function will
   set permanent data of this molecule (eq. secondary structure
   or 3' dangling end etc.). With cashing for RNA folding.

   CAUTION, Do not change or initlize variable attributes of
   TYPE2, e.g. ".bonr", ".bonc" because these can have special
   meaning which should not modified by AnnotMolCash().
 */

/*We immediately complement all the info about the molecule, 
to determine its putative function*/
void AnnotMolCash(TYPE2 *mole)
{
  mole->energy=MyFold(mole->seq,mole->str);
  /* to show that 'mole' doesn't know the structure of parent */
  /*complement info*/
  PerfectReplication(mole->cseq,mole->seq);
  mole->cenergy=MyFold(mole->cseq,mole->cstr);
  mole->kcat = Ptype2kcat(mole->str);
  mole->ckcat=Ptype2kcat(mole->cstr);
  openEnd(mole);
  mole->krec = 1.;
  /*KMUT IS UPDATED IN THE MAIN FILE, as well as sign and ancestry*/
  mole->who=det_phen(mole->seq,mole->cseq,mole->kcat, mole->end5, mole->end3, mole->ckcat, mole->cend5, mole->cend3);
}

/* 
   This function annotates molecule when mutation does not
   happen. If no mutations happen, we do not have to fold 
   anything because the father should contain everything.
   If it doesn't it's actually a problem, because it means that 
   somewhere we're not copying info properly.
   NOTICE THAT SIGN AND ANCESTOR (anc[]) ARE UPDATED IN MAIN
   because they are not molecular features...
   ..ehehe, no, it's just that that's how i made them 
   and don't feel like changing them.
*/
void AnnotMolCashNoMutation(TYPE2 *child,TYPE2 *parent)
{
  strcpy(child->cseq, parent->seq);
  if(parent->cstr[0]!='\0'){
    strcpy(child->str,parent->cstr);	/*copy parent str into child str*/
    strcpy(child->cstr,parent->str);	
    
    strcpy(child->end3,parent->cend3);	/*dangling ends*/
    strcpy(child->end5,parent->cend5);
    child->kcat = parent->ckcat;	/*Catalytic-ness*/
    
    strcpy(child->cend3,parent->end3);
    strcpy(child->cend5,parent->end5);
    child->ckcat = parent->kcat;
    
    child->energy = parent->cenergy;	/*copy energies*/
    child->cenergy = parent->energy;
    
    child->kmut = parent->kmut;
    
    child->who=parent->who;	/*since no mut happened, functional class must be the same*/
  
  }else{
    fprintf(stderr, "AnnotMolCashNoMutation(). Error: Unknown parent->cstr.\n");
    exit(1);
  }
  child->krec = 1.;  
}

void Get3End(char *end3,char* seq,char *str)
{
  int i;
  for(i=0;i<MAXLEN;i++){
    if(str[i]=='.')
      end3[i]=seq[i];
    else{
      end3[i]='\0';
      break;
    }
  }
}

/* 
   To determine the type of molecules, we need to know not only
   its structure, but also the structure of its complementary
   sequence. However, we do not have to know the structure of the
   complementary sequence when we update the plane. Thus, during
   the simulation, to know the structure of the complementary
   sequence is nothing but overhead. However, once a while we
   plot the population size, and then we need to know the
   structure of the complementary sequence. The following
   function will determine the structure of the complementary
   sequence.
*/
void AnnotPlane(TYPE2 **world)
{
  int i,j;
  int nr=nrow, nc=ncol;
  char cseq[MAXLEN];
  for(i=1;i<=nr;i++)
    for(j=1;j<=nc;j++){
      if(world[i][j].seq[0]!='N' && world[i][j].cstr[0]=='\0'){
	PerfectReplication(cseq,world[i][j].seq);
	world[i][j].cenergy = MyFold(cseq,world[i][j].cstr);
      }
    }
}

void openEnd(TYPE2 *mole)
{
  int i,j;
 /* 3' end */
  for(i=0;i<MAXLEN;i++){
    if(mole->str[i]=='.')
      mole->end3[i]=mole->seq[i];
    else{
      mole->end3[i]='\0';
      break;
    }
  }

  /* 5' end */
  for(i=1,j=strlen(mole->seq);i<MAXLEN;i++){
    if(mole->str[j-i]=='.')
      mole->end5[i-1]=mole->seq[j-i];
    else {
      mole->end5[i-1]='\0';
      break;
    }
  }
  
  /*compl seq 3' end */
  for(i=0;i<MAXLEN;i++){
    if(mole->cstr[i]=='.')
      mole->cend3[i]=mole->cseq[i];
    else{
      mole->cend3[i]='\0';
      break;
    }
  }

  /*compl seq 5' end */
  for(i=1,j=strlen(mole->cseq);i<MAXLEN;i++){
    if(mole->cstr[j-i]=='.')
      mole->cend5[i-1]=mole->cseq[j-i];
    else {
      mole->cend5[i-1]='\0';
      break;
    }
  }
  
}

/* Give the folding of a molecule. It will give back the prob. of
   catalysis. */
double Ptype2kcat(char * str)
{
  static int firstcall=1;
  char *b2c;
  static char *target_expand_shapiro;
  char *expand_shapiro;
  int dist;

  if(firstcall==1){  
    b2c=b2C(par_target_structure);
    target_expand_shapiro=expand_Shapiro(b2c);
    free(b2c);
    firstcall = 0;
  }

  
  b2c=b2C(str);
  expand_shapiro=expand_Shapiro(b2c);

  dist=strcmp(target_expand_shapiro,expand_shapiro);

  free(b2c);
  free(expand_shapiro);

  if(dist==0)
    return par_kcat;
  else
    return 0.;
}

/* Give a free end "end3" and "end5". It will calculate the
   prob. of association and dissociation. */
void AssoDissRate(char *end3, char *end5, double *k_asso, double *k_diss)
{
  int maxmatch;
  static double k_a[100];
  static double k_d[100];
  static int if_first_call=1;
  int i;

  if(if_first_call==1){
    for(i=0;i<100;i++){
      k_a[i] = 1. - exp(par_factor_asso*i);
      k_d[i] = exp(par_factor_diss*i);
    }
    if_first_call=0;
  }

  maxmatch = MaxMatch(end3,end5);
  if(maxmatch<100){
    *k_asso = k_a[maxmatch];
    *k_diss = k_d[maxmatch];
  }
  else{
    *k_asso = 1. - exp(par_factor_asso*(double)maxmatch);
    *k_diss = exp(par_factor_diss*(double)maxmatch);
  }
}


/* This will give back the maximam number of base pairs between
   "end3" and "end5". We count GC as 3, AU as 2, GU as 1. */
int MaxMatch(char *end3, char *end5)
{
  int i,j,len3,len5,len_diff;
  int nmatch; /* current */
  int pnmatch=0; /* previous */
  char *end_last,*end_middle; /* dont be confused by the name. */

  len3=strlen(end3);
  len5=strlen(end5);

  if(len3==0 || len5==0)
    return 0;

  if(len3>=len5) {
    len_diff = len3 - len5;
    end_last = end5 + len5;
    end_middle = end3 + len_diff;
    /*
      Let the length of 3' be 'll' and 5' be 'dd'.
      Let l = ll-1 and d = dd-1.
      "end_last" is pointing to "5[dd]".
      (i=1)
      3'       012........l   -> 3[0] is the begining
      5' 012...d              -> 5[dd-i] is the begining

      (i=dd)
      3' 0123........l        -> 3[0]
      5' 0123.....d           -> 5[dd-dd]
    */
    for(i=1,j=len5;i<=j;i++){
      nmatch = nbasepair(end3,end_last-i,i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    /*
      (i=1)
      3' 0123.........l     -> 3[i]
      5'  0123....d         -> 5[0]

      (i=ll-dd)
      3' 0123.........l     -> 3[(ll-dd)]
      5'      0123....d     -> 5[0]
    */
    for(i=1,j=len_diff;i<=j;i++){
      nmatch = nbasepair(end3+i,end5,len5);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    /*
      Let "end_middle" point to "3[(ll-dd)]".
      (i=1)
      3' 0123.........l      -> 3[(ll-dd)+i]
      5'       0123....d     -> 5[0]

      (i=dd-1)
      3' 0123........l            -> 3[(ll-dd)+dd-1] = 3[l]
      5'             0123.....d   -> 5[0]
    */
    for(i=1,j=len5-1;i<=j;i++){
      nmatch = nbasepair(end_middle+i,end5,len5-i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    return pnmatch;
  }
  else {
    len_diff = len5 - len3;
    end_last = end3 + len3;
    end_middle = end5 + len_diff;
    for(i=1,j=len3;i<=j;i++){
      nmatch = nbasepair(end5,end_last-i,i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    for(i=1,j=len_diff;i<=j;i++){
      nmatch = nbasepair(end5+i,end3,len3);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    for(i=1,j=len3-1;i<=j;i++){
      nmatch = nbasepair(end_middle+i,end3,len3-i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
  }
  return pnmatch;
}

/* Sum over the base pair between "this" and "that". Don't forget
   the length of the sequence we want to look. */
int nbasepair(char* this,char* that,int len)
{
  int i;
  int nmatch=0;

  for(i=0;i<len;i++){
    nmatch+=ifbasepair(*(this+i),*(that+i));
  }

  return nmatch;
}
/* If bases can make a pair, return 1(GU) or 2(AU) or 3(GC),
   other wise 0 */
int ifbasepair(char this,char that)
{
  switch (this) {
  case 'A': {
    switch(that) {
    case 'A': return 0;
    case 'C': return 0;
    case 'G': return 0;
    case 'U': return 2;
    default : return 0;
    }
  }
  case 'C': {
    switch(that) {
    case 'A': return 0;
    case 'C': return 0;
    case 'G': return 3;
    case 'U': return 0;
    default : return 0;
    }
  }
  case 'G': {
    switch(that) {
    case 'A': return 0;
    case 'C': return 3;
    case 'G': return 0;
    case 'U': return 1;
    default : return 0;
    }
  }
  case 'U': {
    switch(that) {
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

/* Max possible base pair value the given sequence could make */
int maxbasepair(char* end3)
{
  int i;
  int max=0;
  for(i=0;*(end3+i)!='\0';i++){
    switch(*(end3+i)) {
    case 'A': case 'U': max+=2; break;
    case 'G': case 'C': max+=3; break;
    }
  }
  return max;
}

/* Use binary tree (cashing) to fold RNA. If there is not entry
   which user want to fold, it fold by fold() and insert that in
   binary tree. 

   Return: energy
*/
double MyFold(char *seq,char *str)
{
  statusEnum status;
  double energy;
  nodeType *result;

  /* debug */
  ncalls_myfold++;

  if((status=binInsertP(seq,&result))==STATUS_DUPLICATE_KEY){

    /* debug */
    nsucc_foldcash++;

    strcpy(str,result->shape);
    return result->energy;
  }else if(status==STATUS_OK){
    energy=(double)fold(seq,str);
    /*insert this seq in the binary tree genotype data base*/
    strcpy(result->shape,str);
    result->energy=energy;

    return energy;

  }

  /* This is error. */
  else{
    fprintf(stderr,"MyFold() binInsertP() returns unknown\n");
    return 1.;
  }
}

/*PAY ATTENTION THAT 3' AND 5' ARE INVERTED!*/
char det_phen(char *seq,char*cseq,double kcat, char *end5, char *end3, double ckcat, char *cend5, char *cend3)
{
  int d3=0,d5=0,c3=0,c5=0;
  
  if((strcmp(seq,Ccat)==0) || (strcmp(seq,c_Ccat)==0)) return 'C';
  else{
  /*Here we correct for the japanese reversal
    which is a bit different from the russian reversal
    but still quite dreadful*/
    d5=MaxBasePairScore(end3);
    d3=MaxBasePairScore(end5);
    c5=MaxBasePairScore(cend3);
    c3=MaxBasePairScore(cend5);
  
  
/*  fprintf(stderr, "%s\t%s\n%s\t%s\n%s\t%s\n", seq1,cseq1,str1,cstr1,sha1,csha1);
   fprintf(stderr, "d5 %d, d3 %d, dc %d, c5 %d, c3 %d, cc %d.\n", d5,d3,dc,c5,c3,cc);
*/
    if(1){	       /*0         1         2         3         4         5         6   */
      char phe_array[]={"jjjjshjojjjjshjojjppsheujjppsheusssssctbhhhhchlajjeetlevoouubavu"};
      return (phe_array[(int)( d5*pow(2,5) + d3*pow(2,4) + kcat*pow(2,3) + c5*pow(2,2) + c3*(2) + ckcat )]);
    }
    if(0){
      char phe_array[]={"jjjjshjojjjjshjojjppsheujjppsheusssssctbhhhhchlajjeetlpooouutlou"};
      return (phe_array[(int)( d5*pow(2,5) + d3*pow(2,4) + kcat*pow(2,3) + c5*pow(2,2) + c3*(2) + ckcat )]);
    }
  }
}

/*takes a seq and its complementary, return min hamming 
distance of the two with respect to another sequence*/
int min_hd_chd(char *seq1,char *cseq1,char *seq2)
{
  int hd=0,c_hd=0,len;
  unsigned int l;

  len=strlen(seq1);
  if(len!=strlen(seq2)){
    fprintf(stderr,"Function hamming distance: sequences have unequal length.\n");
    exit(1);
  }
  
  for(l=0;l<len;l++){
    if(seq1[l] != seq2[l]) hd++;
    if(cseq1[l] != seq2[l]) c_hd++;
  }
  hd = (hd < c_hd)? hd : c_hd;		  /*assign to hd the smallest btwn hd and c_hd*/
  return hd;
}

int MaxBasePairScore(char *end)
{
  int i;
  int max=0;
  for(i=0;*(end+i)!='\0';i++){
    switch(*(end+i)) {
    case 'A': case 'U': max+=2; break;
    case 'G': case 'C': max+=3; break;
    }
  }
  if(max>=par_m_threshold) return (1);
  else return (0);
}
