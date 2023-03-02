/* 
   Binary tree (clumping genotypes in one phnotype) We use the
   different binary tree as RNA folding cashing and RNA
   population tracking. A node of the tree contains information
   of the phenotype of (+) string and (-) string. The first
   elements of each of the attribute for phenotype is for
   (+)string, the second element is for (-)string. The difinition
   of (+) and (-) strings are determined by comparing structures,
   end5 sequences and end3 sequences in the order stated. If
   every thing is the same, we choose (+) string arbitrarily.
   See phIfPlus() for details.
*/

#include "foldcash.h"


#ifndef PHENOTREE
#define PHENOTREE


#define PLUS_STRING 0
#define MINUS_STRING 1
typedef struct phenotype_nodeTag {
  struct phenotype_nodeTag *left;
  struct phenotype_nodeTag *right;
  struct phenotype_nodeTag *parent;
  /* 'str', 'end3' and 'end5' are all key. */
  char str[2][MAXLEN]; /* [0]:(+)string, [1]:(-)string*/
  char end3[2][MAXLEN];
  char end5[2][MAXLEN];
  /* the followings are attributes */
  int npop[2]; /* It will get poulation size of this phenotype [0]:+ [1]:1 */
  //  int cseq[MAXLEN][4]; /* Consensus sequence. A:0 C:1 G:2 U:3 */
  char master[2][MAXLEN]; /* most abundant sequence (master seq.) [0]:+, [1]:- */
  int nmaster[2]; /* the pop size of master sequence */
  //double shannon[2]; /* shannon information = Sigma p ln(p) where p is a frequence of a genotype */
}phNodeType; /* 'ph'represents phenotype */


statusEnum phInsert(phNodeType **,phNodeType *);
statusEnum phDeleteTree(phNodeType*);
statusEnum phResetTree(phNodeType*);
int phCount(TYPE2*);
phNodeType *phTopn(void);
int phCmp2(char [][MAXLEN],char [][MAXLEN],char [][MAXLEN],char [][MAXLEN],char [][MAXLEN],char [][MAXLEN]);

#endif //PHENOTREE
