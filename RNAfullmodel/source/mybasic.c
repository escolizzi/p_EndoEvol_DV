#include <stdio.h>
#include <string.h>
#include "cash2.h"
#include "cash.h"


extern FILE *parfile;

extern int seed;
extern int seedset;
extern int graphics;
extern int nrow;
extern int ncol;
extern int first;
extern int last;
extern int boundary;
extern int boundaryvalue;
extern int scale;
extern int ranmargolus;
extern int psborder;
extern int psreverse;

MYTYPE **MyNewP(void)
{
  MYTYPE **a;
  a = (MYTYPE **)calloc(nrow+2, sizeof(MYTYPE *));
  if (a == NULL) {
    fprintf(stderr,"MyNewP: error in memory allocation\n");
    exit(1);
  }
  return a;
}

MYTYPE **MyNew(void)
{
  MYTYPE **a;
  int i,j;
  a = MyNewP(); 
  a[0] = (MYTYPE *)calloc((nrow+2)*(ncol+2),sizeof(MYTYPE));
  if (a[0] == NULL) {
    fprintf(stderr,"MyNew:error in memory allocation\n");
    exit(1);
  }
  for (i=1,j=nrow+2; i < j; i++)
    a[i] = a[i-1] + ncol + 2;

  if (first == 0) {
    first = ncol + 3;
    last = (nrow+2)*(ncol+2) - ncol - 4;
  }
  return a;
}

int MyPlaneFree(MYTYPE **a)
{
  int i,j;
  /*$dir force_vector*/
  for (i=0,j=nrow+2; i < j; i++)
    free(a[i]);
  free(a);
  return 0;
}
