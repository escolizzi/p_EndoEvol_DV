#include <string.h>
#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "mersenne.h"
#include "rna_string.h"
/*
  Give a parent sequence, it will make a child sequence with
  mutations. Give also mutatoin rate in "mut". It will return the
  number of mutations.
*/
int Replication(char* child,char* parent, double mut)
{
  int rna_site[MAXLEN]; /* We chose one of the elements of this
			    array. The value stored in the chosen
			    element is the site of mutation */
  int rna_len;

  int nmut; /* the number of mutations */
  int end; /* These represent the range of the site numbers of RNA
	      string from which we chose sites mutating */
  int msite[MAXLEN]; /* A site number where we will insert a
			point mutation */
  int index; /* Index of 'rna_site'. The element of corresponding
		element stores the position of the
		mutation. Index is determined by calling random
		number generator. */
  int mint; /* It will take 1,2,3 uniformely randomly. Think
	       about a room such that A->C->U->T->A(repeat). We
	       incliment this room from the original character by
	       'mint'. This means that we rondomly chose one of
	       the 3 characters */
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
    index=genrand_real3()*(double)(end);

    msite[i] = rna_site[index];

    /* We put the site of mutation in the last element of
       "rna_site" into the choice range, i.e., rna_site[end-1]
       into rna_site[index] in order that we do not chose the
       site in rna_site[index] twice and that we can chose a site
       in rna_site[end-1] later. */
    rna_site[index] = rna_site[end-1];

    /* decrement "end". We should not chose rna_site[end-1] again
       where "end" is before the decrement because it has been
       already chosen. After the decrement, rna_site[end-1] can
       be chosen. */
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
  int i,end;
  int length;

  length = strlen(parent);
  end = length - 1;
  for(i=0;i<length;i++){
    child[i] = RNAc2cc(parent[end-i]);
  }
  child[length] = '\0';
}

/* If two RNA sequences are complimentary sequences, it gives
   back 0. Otherwise, something else . */

int IfcRNA(char *str1,char *str2)
{
  char c_str2[MAXLEN];
  PerfectReplication(c_str2,str2);
  return (strcmp(c_str2,str1));
}

/************************
 * RNAi2c, c2i and c2cc *
 ************************/

/* char RNAi2c(int rnaint)
   Parameter: give a RNA digit in 'rnaint' to change it to RNA character.
   Description: RNA ind to character.
   Return value: 'A' for 0, 'C' for 1, 'G' for 2, 'U' for '3', '\0' for error. 
   Note: Better to use macro may be. 
*/
char RNAi2c(int rnaint)
{
  switch (rnaint) {
  case 0: 
    return ('A');
  case 1:
    return ('C');
  case 2:
    return ('G');
  case 3:
    return ('U');
  default:
    fprintf(stderr,"RNAi2c: I got a non RNA interger");
    return ('\0');
  }
}

/* int RNAc2i(char rnachar)
   Parameter: give a RNA character (ACGU).
   Description: RNA character to RNA integer
   Return value: 0 for 'A', 1 for 'C', 2 for 'G', 3 for 'U', -1 for error..
*/
int RNAc2i(char rnachar)
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
    fprintf(stderr,"RNAc2i: I got a non RNA character");
    return -1;
  }
}

char RNAc2cc(char rna)
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
    fprintf(stderr,"RNAcc2c: I got a non RNA character");
    return '\0';
  }
}
