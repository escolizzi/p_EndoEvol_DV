#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "foldcash.h"


/****************************
 *       Binary Tree        *
 ****************************/

static nodeType *root = NULL;          /* root of binary tree */
static int nNode=0;

int binGetNNode(void)
{
  return nNode;
}

/* This version of binInsertP() gives you back a pointer to the
   node you are going to insert. So, after calling this, don't
   forget to set the attributes of the node */
statusEnum binInsertP(char *key, nodeType **result) {
  nodeType *x, *current, *parent;
  int comp=0;

  /* error check */
  if(key[0]=='\0'){
    printf("binInsert: Got a null argument\n");
    return  STATUS_MEM_EXHAUSTED;
  }

  /* find future parent */
  current = root;
  parent = NULL;
  while (current!=NULL) {
    comp = strcmp(key, current->key);
    if (comp == 0) {
      *result = current;
      return STATUS_DUPLICATE_KEY;
    }
    parent = current;
    current = comp < 0 ? 
      current->left : current->right;
  }

  /* If this entry (current) is not existing, allocate the memory
     and insert the data */

  /* setup new node */
  x = calloc (1,sizeof(nodeType));
  if (x == NULL) {
    return STATUS_MEM_EXHAUSTED;
  }
  /* count the # of node */
  nNode++;
  /* insert x in tree */
  x->parent = parent;
  x->left = NULL;
  x->right = NULL;
  if(parent!=NULL)
    if(comp < 0)
      parent->left = x;
    else
      parent->right = x;
  else
    root = x;

  /* set the key */
  strcpy(x->key, key);
  /* rest of the attributes must be set by user */
  *result = x;

  return STATUS_OK;
}

/* Delete a node if "key" exists in the tree */
statusEnum binDelete(char *key) {
  nodeType *x, *y, *z;
  int comp;
  /***************************
   *  delete node from tree  *
   ***************************/

  /* find node in tree */
  z = root;
  while(z != NULL) {
    comp = strcmp(key, z->key);
    if(comp == 0) 
      break;
    else
      z = comp < 0 ? z->left : z->right;
  }
  if (!z) return STATUS_KEY_NOT_FOUND;

  /* find tree successor */
  if (z->left == NULL || z->right == NULL)
    y = z;
  else {
    y = z->right;
    while (y->left != NULL) y = y->left;
  }

  /* x is y's only child */
  if (y->left != NULL)
    x = y->left;
  else
    x = y->right;

  /* remove y from the parent chain */
  if (x) x->parent = y->parent;
  if (y->parent)
    if (y == y->parent->left)
      y->parent->left = x;
    else
      y->parent->right = x;
  else
    root = x;

  /* if z and y are not the same, replace z with y. */
  if (y != z) {
    y->left = z->left;
    if (y->left) y->left->parent = y;
    y->right = z->right;
    if (y->right) y->right->parent = y;
    y->parent = z->parent;
    if (z->parent)
      if (z == z->parent->left)
	z->parent->left = y;
      else
	z->parent->right = y;
    else
      root = y;
    free (z);
  } else {
    free (y);
  }
  return STATUS_OK;
}

statusEnum binFind(char *key, char *shape) {
  int comp;
  nodeType *current = root;
  /*******************************
   *  find node containing data  *
   *******************************/

  /* error check */
  if(key[0]=='\0'){
    printf("binFind: I got a null argument\n");
    return   STATUS_MEM_EXHAUSTED;
  }


  while(current!=NULL) {
    comp = strcmp(key, current->key);
    if(comp == 0) {
      strcpy(shape, current->shape);
      return STATUS_OK;
    } else {
      current = comp < 0 ? 
	current->left : current->right;
    }
  }
  return STATUS_KEY_NOT_FOUND;
}

statusEnum binFindP(char *key, nodeType** result) {
  int comp;
  nodeType *current = root;
  /*******************************
   *  find node containing data  *
   *******************************/

  /* error check */
  if(key[0]=='\0'){
    printf("binFindP: I got a null argument\n");
    return   STATUS_MEM_EXHAUSTED;
  }


  while(current!=NULL) {
    comp = strcmp(key, current->key);
    if(comp == 0) {
      *result=current;
      return STATUS_OK;
    } else {
      current = comp < 0 ? 
	current->left : current->right;
    }
  }
  result = NULL;
  return STATUS_KEY_NOT_FOUND;
}
/* Delete an entire tree */
statusEnum binDeleteTree(nodeType *current) {
  nodeType *parent;
  /**************************
   *  empty entire tree     *
   **************************/
  if (current == NULL)	{
    current = root;
  }
  if (current == NULL)	{ // if root==NULL, entire tree has been deleted
    return STATUS_OK;	
  }
  // current contains node to delete
  if (current->left != NULL)
    binDeleteTree(current->left);
  if (current->right != NULL)
    binDeleteTree(current->right);
  parent = current->parent;
  if (parent != NULL)	{
    if (current == parent->left)
      parent->left = NULL;
    else
      parent->right = NULL;
  }
  if (current == root)
    root = NULL;
  free(current);
  current = NULL;
  return STATUS_OK;
}

void binRenew(TYPE2 **world,int nr,int nc)
{
  int i,j;
  nodeType *result;

  binDeleteTree(NULL);
  nNode = 0;

  for(i=1;i<=nr;i++)
    for(j=1;j<=nc;j++){
      if(world[i][j].seq[0]!='N'){
	binInsertP(world[i][j].seq,&result);
	if(result->shape[0]=='\0'){
	  strcpy(result->shape,world[i][j].str);
	  result->energy = world[i][j].energy;
	}
      }
    }
}

