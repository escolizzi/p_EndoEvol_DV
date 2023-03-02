#include "constants.h"
#include "cash2.h"

#ifndef FOLDCASH
#define FOLDCASH



/* Binary tree (cashing of RNA folding) */
typedef enum {
  STATUS_OK,
  STATUS_MEM_EXHAUSTED,
  STATUS_DUPLICATE_KEY,
  STATUS_KEY_NOT_FOUND
} statusEnum;

typedef struct nodeTag {
  struct nodeTag *left;  /* left child */
  struct nodeTag *right;  /* right child */
  struct nodeTag *parent;  /* parent */
  char key[MAXLEN];  /* key used for searching. sequence of RNA */
  char shape[MAXLEN];  /* shape (folding) of the rna */
  double energy; /* energy */
} nodeType;

/* not to make grobal variable across the files */
int binGetNNode(void);

statusEnum binInsertP(char *, nodeType **);
statusEnum binDeleteTree(nodeType *);
void binRenew(TYPE2**,int nr,int nc);


#endif //FOLDCASH
