#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "cash.h"
#include "cash2.h"
#include "phenotree.h"
#include "my.h"
#include "rna_string.h"
#include "annotmol.h"

/*************************
 * Phenotype Binary Tree *
 *************************/

static phNodeType *proot = NULL; /* catalyst */
//static phNodeType *proot_par = NULL; /* parasite */
static int nPhNode=0;

int phCmp(phNodeType *,phNodeType *);
int phIfPlusString(char *,char *,char *,char *,char *,char *);

/* To make a binary tree of phenotype, the function will decide
   if the phenotype "a" a "larger than" or "equal to" or "smaller
   than" the phenotype "b". 
*/
int phCmp(phNodeType *a,phNodeType *b)
{
  int dif;
  /* First, we compare PLUS_STRING */
  if((dif=strcmp(a->str[PLUS_STRING],b->str[PLUS_STRING]))!=0)
    return dif;

  /* end5 useally always exists. So we compare them next */
  if((dif=strcmp(a->end5[PLUS_STRING],b->end5[PLUS_STRING]))!=0)
    return dif;

  if((dif=strcmp(a->end3[PLUS_STRING],b->end3[PLUS_STRING]))!=0)
    return dif;
  
  if((dif=strcmp(a->str[MINUS_STRING],b->str[MINUS_STRING]))!=0)
    return dif;

  if((dif=strcmp(a->end5[MINUS_STRING],b->end5[MINUS_STRING]))!=0)
    return dif;

  if((dif=strcmp(a->end3[MINUS_STRING],b->end3[MINUS_STRING]))!=0)
    return dif;

  return 0;
}



int phCmp2(char astr[][MAXLEN],char aend5[][MAXLEN],char aend3[][MAXLEN],char bstr[][MAXLEN],char bend5[][MAXLEN],char bend3[][MAXLEN])
{
  int dif;
  /* First, we compare PLUS_STRING */
  if((dif=strcmp(astr[PLUS_STRING],bstr[PLUS_STRING]))!=0)
    return dif;

  /* end5 useally always exists. So we compare them nest */
  if((dif=strcmp(aend5[PLUS_STRING],bend5[PLUS_STRING]))!=0)
    return dif;

  if((dif=strcmp(aend3[PLUS_STRING],bend3[PLUS_STRING]))!=0)
    return dif;
  
  if((dif=strcmp(astr[MINUS_STRING],bstr[MINUS_STRING]))!=0)
    return dif;

  if((dif=strcmp(aend5[MINUS_STRING],bend5[MINUS_STRING]))!=0)
    return dif;

  if((dif=strcmp(aend3[MINUS_STRING],bend3[MINUS_STRING]))!=0)
    return dif;

  return 0;
}

/* 
   In making the binary tree of phenotype, we have to assign a
   genotype to the corresponding phenotype node of the tree. In
   the tree node, there are two entry, one for (+) string and the
   other for (-) string which is complementary to (+). So, we
   need to decide if the genotype is (+) or (-), which is by all
   means arbitrary. We do that by the following function.

   Argument: Give all phenotype features (structure, end5, end3)
   of the focal sequence. And give all phenotype features of the
   comprementary sequence of the focal sequence.

   Return value: if positive, given genotype is (+) string. if
   negative, given genotype is (-) string. if zero, (+) and (-)
   have the same phenotype.
*/
int phIfPlusString(char *focalStr,
		   char *focalEnd5,
		   char *focalEnd3,
		   char *complStr,
		   char *complEnd5,
		   char *complEnd3)
{
  int dif;
  /* First, we compare structure */
  if((dif=strcmp(focalStr,complStr))!=0)
    return dif;

  /* end5 useally always exists. So we compare them next */
  if((dif=strcmp(focalEnd5,complEnd5))!=0)
    return dif;

  if((dif=strcmp(focalEnd3,complEnd3))!=0)
    return dif;

  return 0;
}

/* phInsert() should get a molecule (TYPE2 pointer) of, the
   function will incliment the population of the phenotype which
   is the same as the phenotype of the given molecule. The
   function is supposed to do nothing more than putting the
   pointer to the node of the tree in "result". After calling
   this funtion, the user must do something to the "result".*/
statusEnum phInsert(phNodeType **result,phNodeType *insert)
{
  phNodeType *x, *current, *parent;
  int comp=0;

  /* find future parent */
  current = proot;
  parent = NULL;
  while (current!=NULL) {
    comp = phCmp(insert,current);
    if (comp == 0)
      break;
    parent = current;
    current = comp < 0 ? 
      current->left : current->right;
  }

  /* If this entry (current) is not existing, allocate the memory
     and insert the data */
  if(current == NULL) {
    /* setup new node */
    x = (phNodeType*) malloc (sizeof(phNodeType));
    if (x == NULL) {
      return STATUS_MEM_EXHAUSTED;
    }
    /* the number of node */
    nPhNode++;
    /* attributes for tree connection */
    x->parent = parent;
    x->left = NULL;
    x->right = NULL;
       
    /* insert x in tree */
    if(parent!=NULL)
      if(comp < 0)
	parent->left = x;
      else
	parent->right = x;
    else
      proot = x;

    /* user has to take care the result */    
    *result = x;
    return STATUS_OK;
  }
  /* If this entry is the same as the "key" */
  else if(comp == 0){
    *result = current;
    return STATUS_DUPLICATE_KEY;
  }
  else {
    fprintf(stderr,"bin2Insert: This can't be the case\n");
    return STATUS_MEM_EXHAUSTED;
  }
}

/* The function will delete the entire tree; i.e. it will free
   the memory allocated for the phenotype tree.  */
statusEnum phDeleteTree(phNodeType* current)
{
  if (current == NULL) {
    current = proot;
  }

  /*if root is empty, entire tree has been cleared */
  if(current == NULL)
    return STATUS_OK;	

  /* if current has a child node to clear */
  if(current->left != NULL) {
    phDeleteTree(current->left);
  }
  if(current->right != NULL) {
    phDeleteTree(current->right);
  }


  if(current == proot)
    proot = NULL;
  free(current);
  current=NULL;

  return STATUS_OK;
}

/* 
   The function needs molecule list which is generated by
   bin2Topn(). phCount() reads through the list, and put the
   element in the phenotype tree by using phInsert(). Since
   phInsert() does not modify the node of the tree but returns
   the pointer to the node, phCount() will set the values of the
   node. 

   return -1 on error. 0 on success.
*/
int phCount(TYPE2* moleList)
{
  int i;
  //  int counter,incliment;
  phNodeType *current;
  phNodeType insert;
  TYPE2 cmol;
  statusEnum status;
  //  double p; shannon

  phDeleteTree(NULL);
  nPhNode = 0;

  for(i=0;(moleList+i)->seq[0]!='\0';i++){
    PerfectReplication(cmol.seq,(moleList+i)->seq);
    MyFold(cmol.seq,cmol.str);
    openEnd(&cmol);
    /* if this genotype "(moleList+i)" is (+)string */
    if(phIfPlusString((moleList+i)->str,(moleList+i)->end5,(moleList+i)->end3,cmol.str,cmol.end5,cmol.end3)>=0){
      strcpy(insert.str[PLUS_STRING],(moleList+i)->str);
      strcpy(insert.end5[PLUS_STRING],(moleList+i)->end5);
      strcpy(insert.end3[PLUS_STRING],(moleList+i)->end3);
      strcpy(insert.master[PLUS_STRING],(moleList+i)->seq);
      insert.npop[PLUS_STRING] = (moleList+i)->bonc;

      strcpy(insert.str[MINUS_STRING],cmol.str);
      strcpy(insert.end5[MINUS_STRING],cmol.end5);
      strcpy(insert.end3[MINUS_STRING],cmol.end3);
      strcpy(insert.master[MINUS_STRING],cmol.seq);
      insert.npop[MINUS_STRING] = 0;
    }
    /* if this genotype "(moleList+i)" is (-)string */
    else{
      strcpy(insert.str[MINUS_STRING],(moleList+i)->str);
      strcpy(insert.end5[MINUS_STRING],(moleList+i)->end5);
      strcpy(insert.end3[MINUS_STRING],(moleList+i)->end3);
      strcpy(insert.master[MINUS_STRING],(moleList+i)->seq);
      insert.npop[MINUS_STRING] = (moleList+i)->bonc;

      strcpy(insert.str[PLUS_STRING],cmol.str);
      strcpy(insert.end5[PLUS_STRING],cmol.end5);
      strcpy(insert.end3[PLUS_STRING],cmol.end3);
      strcpy(insert.master[PLUS_STRING],cmol.seq);
      insert.npop[PLUS_STRING] = 0;
    }

    if((status=phInsert(&current,&insert))==STATUS_OK){
      /* initilizing a attribute of the node, shannon */
      //      current->shannon[PLUS_STRING] = current->shannon[MINUS_STRING] = 0.;
      /* setting the other attributes of node */
      memcpy(*(current->str),*(insert.str),sizeof(char)*MAXLEN*2);
      memcpy(*(current->end5),*(insert.end5),sizeof(char)*MAXLEN*2);
      memcpy(*(current->end3),*(insert.end3),sizeof(char)*MAXLEN*2);
      current->npop[PLUS_STRING] = current->nmaster[PLUS_STRING] = insert.npop[PLUS_STRING];
      current->npop[MINUS_STRING] = current->nmaster[MINUS_STRING] = insert.npop[MINUS_STRING];
      memcpy(*(current->master),*(insert.master),sizeof(char)*MAXLEN*2);

      /* Consensus sequence */
      //      inclCSeq(current->cseq,(moleList+i)->seq,(moleList+i)->bonc);
    }
    else if(status == STATUS_DUPLICATE_KEY){
      current->npop[PLUS_STRING] += insert.npop[PLUS_STRING];
      current->npop[MINUS_STRING] += insert.npop[MINUS_STRING];

      /* Here, we might change the master sequence of this
	 phenotype. We have a bug here. The population of the
	 minus string will not be correct; it is always 0. */
      if(current->nmaster[PLUS_STRING] < insert.npop[PLUS_STRING]){
	strcpy(current->master[PLUS_STRING],insert.master[PLUS_STRING]);
	current->nmaster[PLUS_STRING] = insert.npop[PLUS_STRING];
	strcpy(current->master[MINUS_STRING],insert.master[MINUS_STRING]);
	current->nmaster[MINUS_STRING] = insert.npop[MINUS_STRING];
      }

      /* Consensus sequence */
      //inclCSeq(current->cseq,(moleList+i)->seq,(moleList+i)->bonc);
    }
    else{
      fprintf(stderr,"phCount: phInsert() gives me back error\n");
      return -1;
    }
  }

  /* Calculate Shannon-Weaver diversity index */
  //  for(i=0;(moleList+i)->seq[0]!='\0';i++){
  //    PerfectReplication(cmol.seq,(moleList+i)->seq);
  //    MyFold(cmol.seq,cmol.str);
  //    openEnd(&cmol);
  //    /* if this genotype "(moleList+i)" is (+)string */
  //    if(phIfPlusString((moleList+i)->str,(moleList+i)->end5,(moleList+i)->end3,cmol.str,cmol.end5,cmol.end3)>=0){
  //      strcpy(insert.str[PLUS_STRING],(moleList+i)->str);
  //      strcpy(insert.end5[PLUS_STRING],(moleList+i)->end5);
  //      strcpy(insert.end3[PLUS_STRING],(moleList+i)->end3);
  //      strcpy(insert.master[PLUS_STRING],(moleList+i)->seq);
  //      insert.npop[PLUS_STRING] = (moleList+i)->bonc;
  //
  //      strcpy(insert.str[MINUS_STRING],cmol.str);
  //      strcpy(insert.end5[MINUS_STRING],cmol.end5);
  //      strcpy(insert.end3[MINUS_STRING],cmol.end3);
  //      strcpy(insert.master[MINUS_STRING],cmol.seq);
  //      insert.npop[MINUS_STRING] = 0;
  //    }
  //    /* if this genotype "(moleList+i)" is (-)string */
  //    else{
  //      strcpy(insert.str[MINUS_STRING],(moleList+i)->str);
  //      strcpy(insert.end5[MINUS_STRING],(moleList+i)->end5);
  //      strcpy(insert.end3[MINUS_STRING],(moleList+i)->end3);
  //      strcpy(insert.master[MINUS_STRING],(moleList+i)->seq);
  //      insert.npop[MINUS_STRING] = (moleList+i)->bonc;
  //
  //      strcpy(insert.str[PLUS_STRING],cmol.str);
  //      strcpy(insert.end5[PLUS_STRING],cmol.end5);
  //      strcpy(insert.end3[PLUS_STRING],cmol.end3);
  //      strcpy(insert.master[PLUS_STRING],cmol.seq);
  //      insert.npop[PLUS_STRING] = 0;
  //    }
  //    if(phInsert(&current,&insert)==STATUS_DUPLICATE_KEY){
  //      if(insert.npop[PLUS_STRING]>0&&current->npop[PLUS_STRING]>0){
  //	p=((double)insert.npop[PLUS_STRING]/current->npop[PLUS_STRING]);
  //	current->shannon[PLUS_STRING]-= p * log(p) * 0.693147180559945; /* 0.69... is ln(2) */
  //      }
  //      if(insert.npop[MINUS_STRING]>0&&current->npop[MINUS_STRING]>0){
  //	p=((double)insert.npop[MINUS_STRING]/current->npop[MINUS_STRING]);
  //	current->shannon[MINUS_STRING]-= p * log(p) * 0.693147180559945; /* 0.69... is ln(2) */
  //      }
  //    }
  //    else{
  //      fprintf(stderr,"phCount: phInsert() gives me back error\n");
  //      return -1;
  //    }
  //  }

  return 0;
}
      /*
      counter = (MAXLEN+7)/8;
      incliment = 0;
      switch((MAXLEN) % 8)
      {
	case 0: do { current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
        case 7:      current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
	case 6:      current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
	case 5:      current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
	case 4:      current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
	case 3:      current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
	case 2:      current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
	case 1:      current->cseq[incliment][A]=current->cseq[incliment][C]=current->cseq[incliment][G]=current->cseq[incliment][U]=0.;incliment++;
	           }while(--counter>0);
      }
      */


/* phList() simply converts the phenotype tree data to the array
   data. */
statusEnum phList(phNodeType *current,phNodeType* phlist)
{
  int i;

  if (current == NULL) {
    current = proot;
  }

  if(current == NULL)
    return STATUS_OK;	

  /* if current has a child node to list up */
  if(current->left != NULL) {
    phList(current->left,phlist);
  }
  if(current->right != NULL) {
    phList(current->right,phlist);
  }

  if(current->npop[PLUS_STRING]>0||current->npop[MINUS_STRING]>0){
    /* if (phlist+i)->seq[0]!='\0', then this means that i-th
       element is empty */
    for(i=0;(phlist+i)->str[PLUS_STRING][0]!='\0';i++)
      ; /* do nothing */
    *(phlist+i)=*current;
  }
  return STATUS_OK;
}

/* To order the list of phenotypes ("phList" array) in descending
   order according to the size of population */
int phListCmp(const void* x,const void* y)
{
  int xx,yy;
  xx = (((phNodeType*)x)->npop[PLUS_STRING] >=  ((phNodeType*)x)->npop[MINUS_STRING]) ? ((phNodeType*)x)->npop[PLUS_STRING] : ((phNodeType*)x)->npop[MINUS_STRING];
  yy = (((phNodeType*)y)->npop[PLUS_STRING] >=  ((phNodeType*)y)->npop[MINUS_STRING]) ? ((phNodeType*)y)->npop[PLUS_STRING] : ((phNodeType*)y)->npop[MINUS_STRING];

  return(yy-xx);
}

/* The function will cleate the array "phList" (data type is
   phNodeType) which contains the data of the phenotype and is
   ordered in descending way by the population size. */
phNodeType *phTopn(void)
{
  int i;
  int nkind; /* # of kinds of molecules */
  static phNodeType *phlist=NULL;
  static int listSize = 0;
  int testVal=0;

  if(listSize<(nPhNode+1)){
    phlist = (phNodeType*) realloc(phlist,(nPhNode+1)*(sizeof(phNodeType)));
    if(phlist==NULL){
      fprintf(stderr,"phTopn: no memory\n");
      return NULL;
    }
    listSize = nPhNode + 1;
  }
  i = (listSize+7)/8 ; 
  switch(listSize % 8)
    {
    case 0: do { (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
    case 7:      (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
    case 6:      (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
    case 5:      (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
    case 4:      (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
    case 3:      (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
    case 2:      (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
    case 1:      (phlist+testVal)->str[PLUS_STRING][0]='\0';testVal++;
               }while(--i>0);
    }

  /* Get the list of phenotype and population: "phlist" */
  phList(NULL,phlist);

  /* We want to know how many entries in the "phlist". */
  for(nkind=0;(phlist+nkind)->str[PLUS_STRING][0]!='\0';nkind++)
    ; /* do nothing */

  /* If no phenotype is found, then do not proceed, but return
     here */
  if(nkind == 0)
    return phlist;
  
  /* Sort "phlist" in the descending order according to the
     population size. */
  qsort((void*)phlist,(size_t)nkind,sizeof(phNodeType),phListCmp);

  return phlist;
}
