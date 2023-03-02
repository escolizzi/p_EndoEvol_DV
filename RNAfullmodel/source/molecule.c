#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fold.h>
#include <RNAstruct.h>
#include <dist_vars.h>
#include <treedist.h>


#include "molecule.h"

/***************************************
 * Molecule annotation and replication *
 ***************************************/
/* You give a TYPE2 pointer variable, given that "seq" has a
   right sequence (not like empty flag 'N'). The function will
   set permanent data of this molecule (eq. secondary structure
   or 3' dangling end etc.)*/
void AnnotMol(TYPE2 *mole)
{
  char cseq[MAXLEN], cstr[MAXLEN];

  mole->energy=(double)fold(mole->seq,mole->str);
  openEnd(mole);
  mole->kcat = Ptype2kcat(mole->str);

  /* if you don't want to use the extra freedom for parasites,
     make mol->krec=1. */
#ifdef ENERGY_EVOLUTION
  mole->krec = exp(factor_rec*mole->energy);
#else /* if NO_ENERGY_EVOLUTION */
  mole->krec = 1.;
#endif
  /* Here you have to be careful if you change the color coding
     of molecules. I cannot use ColorByFunction() because we
     should not use MyFold() (cashing). */
  PerfectReplication(cseq,mole->seq);
  fold(cseq,cstr);
  if(Ptype2kcat(cstr)>kthreshold){
    if(mole->kcat>kthreshold)
      mole->type = 1;
    else
      mole->type = 3;
  }
  else
    if(mole->kcat>kthreshold)
      mole->type = 2;
    else
      mole->type = 4;
    
    
}

/* This function does the same thing as AnnotMol() does. But with
   cashing for RNA folding.

   CAUTION, Do not change or initlize variable attributes of
   TYPE2, e.g. ".bonr", ".bonc" because these can have special
   meaning which should not modified by AnnotMolCash().
 */
void AnnotMolCash(TYPE2 *mole)
{
  mole->energy=MyFold(mole->seq,mole->str);
  /* if you don't want to use the extra freedom for parasites,
     make mol->krec=1. */
#ifdef ENERGY_EVOLUTION
  mole->krec = exp(factor_rec*mole->energy);
#else /* if NO_ENERGY_EVOLUTION */
  mole->krec = 1.;
#endif
  openEnd(mole);
  mole->kcat = Ptype2kcat(mole->str);
  mole->type = ColorByFunction(mole);
  //mole->type = ColorByGenotype(mole->seq);
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
}

/* Give the folding of a molecule. It will give back the prob. of
   catalysis. */
double Ptype2kcat(char * str)
{
#ifdef HOWTO_FUNCTION_EASY
  static int firstcall=1;
  char *b2c;
  static char *target_expand_shapiro;
  char *expand_shapiro;
  int dist;

  if(firstcall==1){  
    b2c=b2C(targetStr);
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
    return kCatG;
  else
    return 0.;
#elif defined HOWTO_FUNCTION_LOOSE
  static int firstcall=1;
  char *b2c;
  char *expand_shapiro;
  static Tree *targetTree;
  Tree *tree;
  int dist;
  static double kCat[100];
  int i;

  if(firstcall==1){  
    b2c=b2C(targetStr);
    expand_shapiro=expand_Shapiro(b2c);
    targetTree=make_tree(expand_shapiro);
    free(b2c);
    free(expand_shapiro);

    for(i=0;i<100;i++)
      kCat[i]=kCatG*exp(-1.*i);
    
    firstcall = 0;
  }

  
  b2c=b2C(str);
  expand_shapiro=expand_Shapiro(b2c);
  tree=make_tree(expand_shapiro);

  dist=tree_edit_distance(tree,targetTree);

  free(b2c);
  free(expand_shapiro);
  free_tree(tree);

  if(dist<100)
    return kCat[dist];
  else
    return kCatG*exp(factor_cat*(double)dist);

  /************************************************/
  /* FROM HERE, ANOTHER CONDITIONAL COMPILATION   */
  /************************************************/
#elif defined HOWTO_FUNCTION_STRICT
  static int firstcall=1;
  static Tree *targetTree;
  char *expand_full;
  Tree *tree;
  int dist;
  static double kCat[100];
  int i;

  if(firstcall==1){  
    expand_full=(targetStr);
    targetTree=make_tree(expand_full);
    free(expand_full);

    for(i=0;i<100;i++)
      kCat[i]=kCatG*exp(-1.*i);

    firstcall = 0;
  }

  expand_full=(str);
  tree=make_tree(expand_full);

  dist=tree_edit_distance(tree,targetTree);

  free(expand_full);
  free_tree(tree);

  if(dist<100)
    return kCat[dist];
  else
    return kCatG*exp(-1.*(double)dist);
#endif
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
      k_a[i] = 1. - exp(factor_asso*i);
      k_d[i] = exp(factor_diss*i);
    }
    if_first_call=0;
  }

  maxmatch = MaxMatch(end3,end5);
  if(maxmatch<100){
    *k_asso = k_a[maxmatch];
    *k_diss = k_d[maxmatch];
  }
  else{
    *k_asso = 1. - exp(factor_asso*(double)maxmatch);
    *k_diss = exp(factor_diss*(double)maxmatch);
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

  /*  if(this=='A'){
    if(that=='A'){
      return 0;
    }
    else if(that=='C'){
      return 0;
    }
    else if(that=='G'){
      return 0;      
    }
    else if(that=='U'){
      return 2;
    }
    else
      return 0;
  }
  else if(this=='C'){
    if(that=='A'){
      return 0;
    }
    else if(that=='C'){
      return 0;      
    }
    else if(that=='G'){
      return 3;      
    }
    else if(that=='U'){
      return 0;      
    }
    else
      return 0;
  }
  else if(this=='G'){
    if(that=='A'){
      return 0;      
    }
    else if(that=='C'){
      return 3;      
    }
    else if(that=='G'){
      return 0;      
    }
    else if(that=='U'){
      return 1;      
    }
    else
      return 0;
  }
  else if(this=='U'){
    if(that=='A'){
      return 2;      
    }
    else if(that=='C'){
      return 0;      
    }
    else if(that=='G'){
      return 0;      
    }
    else if(that=='U'){
      return 0;      
    }
    else
      return 0;
  }
  else
    return 0;*/
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

/*
  Give a parent sequence, it will make a child sequence with
  mutations. Give also mutatoin rate in "mut". It will return the
  number of mutations.
*/
int Replication(char* child,char* parent, double mut)
{
  int rna_site[MAXLEN]; /* We chose one of the elements of this array. The
			    value stored in the chosen element is the site
			    of mutation */
  int rna_len;

  int nmut; /* the number of mutations */
  int end; /* These represent the range of the site numbers of RNA
	      string from which we chose sites mutating */
  int msite[MAXLEN]; /* A site number where we will insert a point mutation */
  int mint; /* It will take 1,2,3 uniformely randomly. Think about a room
	       such that A->C->U->T->A(repeat). We incliment this room from
	       the original character by 'mint'. This means that we
	       rondomly chose one of the 3 characters */
  int i;
  char original;

  /* Replicate parent to child. We do mutation later. */
  PerfectReplication(child,parent);

  if(mut == 0.)
    return 0;

  /* Here, we calculate how many mutation occurs */
  rna_len = strlen(parent);
  nmut = (int) bnldev(mut,rna_len);

  if(nmut==0)
    return 0;

  /* Before choosing where mutations occur, initialize
     'rna_site'. We use this to chose where mutations occur. */
  for(i=0;i<rna_len;i++)
    rna_site[i]=i;

  /* end will be decremented each time we chose a mutation
     site. */
  end=rna_len;

  /* We chose the sites of mutation */
  for(i=0;i<nmut;i++){
    /* Chose a site */
    /* (the fractional part is truncated) */
    msite[i]=genrand_real3()*(double)(end);

    /* We put the site residing in the last element of "rna_site"
       within the choice range, i.e., rna_site[end-1] into
       rna_site[msite] in order that we do not chose the site in
       rna_site[msite] twice and that we can chose a site in
       rna_site[end-1] later. */
    rna_site[msite[i]] = rna_site[end-1];

    /* decrement "end". We should not chose rna_site[end-1] where
       "end" is before the decrement. After the decrement,  */
    end--;
  }

  /* We decide what kind of mutation occurs at chosen sites where
     mutation will occur. Then substitute the bases in the child
     sequence. */
  for(i=0;i<nmut;i++){
    original = child[msite[i]];
    mint=(int)floor(genrand_real2()*3.0)+1; /* 'mint' is 1,2,3 */
    child[msite[i]] = RNAi2c((RNAc2i(original)+mint)%4);
  }

  return nmut;
}

void PerfectReplication(char *child,char *parent)
{
  char pre_child[MAXLEN];
  int i,end;
  int length;

  length = strlen(parent);

  for(i=0;i<length;i++){
    pre_child[i] = RNAc2cc(parent[i]);
  }


  /* convert the 3' and 5' order */
  end = length - 1;
  for(i=0;i<length;i++){
    child[i] = pre_child[end-i];
  }
  child[i] = '\0';
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


  if((status=binFindP(seq,&result))==STATUS_OK){
    strcpy(str,result->shape);
    return result->energy;
  }
  else if (status==STATUS_KEY_NOT_FOUND){
    energy=(double)fold(seq,str);
    /*insert this seq in the binary tree genotype data base*/
    binInsertP(seq,&result);
    strcpy(result->shape,str);
    result->npop=0;
    result->energy=energy;

    return energy;
  }
  /* This is error. */
  else
    return 1.;
}

/* If two RNA sequences are complimentary sequences, it gives
   back 0. Otherwise, something else . */

int IfcRNA(char *str1,char *str2)
{
  char c_str2[MAXLEN];
  PerfectReplication(c_str2,str2);
  return (strcmp(c_str2,str1));
}


/* 
   The function can specify different color for predefined
   genotypes. This is usefull for ecological experiment. Use this
   function in AnnotMolCash() is efficient than in MyDisplay().
*/
int ColorByGenotype(char* seq)
{
  /* predifined genptypes */
  char genotype[6][MAXLEN] = 
    {
      "CCCACCGCCGAGGACAUCUAUUGUAGUUAUUCCUACGUAAUUAAGACCGUUGUACGGUCGACCAGAGAUUUCACUCUAACCUCCAGUCAGUUGCUUGCUC",/* t8090000 0+ indigo */
      "GAGCAAGCAACUGACUGGAGGUUAGAGUGAAAUCUCUGGUCGACCGUACAACGGUCUUAAUUACGUAGGAAUAACUACAAUAGAUGUCCUCGGCGGUGGG",/* 1- */
      "CCCACCGUCGAGGACAUCUAUUGUAGUUAAACCUACGUAAUUUAGACCGUUGUACGGUCGACCAGAGAUUUCACUCUAACCUCCAGUCAGUUGCUUGCUC",/* t8090000 4+ turquoise */
      "GAGCAAGCAACUGACUGGAGGUUAGAGUGAAAUCUCUGGUCGACCGUACAACGGUCUAAAUUACGUAGGUUUAACUACAAUAGAUGUCCUCGACGGUGGG",/* 4- */
      "CCCACCGUCGAGGACAUCUAUUGUAGUUAAACCUACGUAAUUUGGACCAUUGUACGGUCGGCCAGAGAUUUCACUCUAACCUCCAGUCAGUUGCUUGCUC",/* t7630000 0+ green */
      "GAGCAAGCAACUGACUGGAGGUUAGAGUGAAAUCUCUGGCCGACCGUACAAUGGUCCAAAUUACGUAGGUUUAACUACAAUAGAUGUCCUCGACGGUGGG"/* 0-  */
    };
  
  if(strcmp(genotype[0],seq)==0||strcmp(genotype[1],seq)==0)
    return 1;
  else if(strcmp(genotype[2],seq)==0||strcmp(genotype[3],seq)==0)
    return 2;
  else if(strcmp(genotype[4],seq)==0||strcmp(genotype[5],seq)==0)
    return 4;
  else{
    fprintf(stderr,"ColorByGenotype: I got a genotypes which is not defined\n");
    return -1;
  }
}

/* This function sets the 'type' attribute of molecules according
   to the function of molecules. Look the following table.
   (+) means functional. 'mole' is a given molecule. 'c-mole' is
   its complementary molecule. "Functional" means here that a
   molecule has catalitic activity and has open 3' end.
   mole=(+) & c-mole=(+) then 1
   mole=(+) & c-mole=(-) then 2
   mole=(-) & c-mole=(+) then 3
   mole=(-) & c-mole=(-) then 4

   Argument: the function needs TYPE2* as an argument. In TYPE2*,
   seq, str, end3 and kcat have to be set before.
*/
int ColorByFunction(TYPE2* mol)
{
  TYPE2 cmol;
  PerfectReplication(cmol.seq,mol->seq);
  MyFold(cmol.seq,cmol.str);
  openEnd(&cmol);
  if(Ptype2kcat(cmol.str)>kthreshold && maxbasepair(cmol.end3)>mthreshold){
    if(mol->kcat>kthreshold && maxbasepair(mol->end3)>mthreshold)
      return 1;
    else
      return 3;
  }
  else
    if(mol->kcat>kthreshold && maxbasepair(mol->end3)>mthreshold)
      return 2;
    else
      return 4;
}
