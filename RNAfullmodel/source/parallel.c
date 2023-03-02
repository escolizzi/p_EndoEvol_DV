#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fold.h>

#include "cash2.h"
#include "mersenne.h"
#include "parallel.h"
#include "foldpipe.h"
#include "foldcash.h"

/* declaration of external functions */
void openEnd(TYPE2 *);
double Ptype2kcat(char *);
int maxbasepair(char*);
void PerfectReplication(char*,char*);
void AssoDissRate(char *,char *,double *,double *);
int Replication(char*,char*,double);

/* declaration of external variables */
extern double kCatG; /* used in Ptype2kcatPipe() */
extern int mthreshold; /* used in ColorByFunctionThread() */


/*******************
 * type definition *
 *******************/

/* This is will be a list of cells waiting to be annotated. This
   is a linked list. Use annotListPush() to push one element,
   annotListPop() to get one element out, annotListDelete() to
   delete a element. */
typedef struct __AnnotList {
  /* where is the place to annotate molecules */
  struct point pos; 
  /* pointer to next */
  struct __AnnotList *next;
  /* pointer to previous */
  struct __AnnotList *prev;
} AnnotList;

/* MutexPlane's coorinate corresponds to that of world. It
   prevents the mixed access to the cells of the world */
typedef struct __MutexPlane {
  pthread_mutex_t mutex;
  /* pointer to the element of AnnotList. This is also a flag
     such that if it is not NULL, then we must annotate this. */
  AnnotList *annotlist;
} MutexPlane;

/**********************************
 * internal function declaration. *
 **********************************/
static MutexPlane **mutexPlaneInitP();
static AnnotList *annotListPush(struct point new_pos);
static int annotListPop(struct point *get);
static void annotListDelete(AnnotList *delete);
static MutexPlane **mutexPlaneInit();
static void mutexInit(void);
static void *annotByThread(void *world_void);
static double MyFoldThread(char *seq,char *str,float (*fold_fnc)(char *,char *));
static void AnnotMolCashByThread(TYPE2 *mole,float (*fold_fnc)(char*,char*),double (*ptype2kcat)(char *));
static int ColorByFunctionThread(TYPE2* mol,float (*fold_fnc)(char*,char*),double (*ptype2kcat)(char *));
static double Ptype2kcatPipe(char *structure);


extern int nrow;
extern int ncol;

/* thread id holder. */
static pthread_t thread_id;

/* plane of mutex */
static MutexPlane **mutex_plane=NULL;

/* list of folding */
static AnnotList *annot_list_head=NULL;
static AnnotList *annot_list_tail=NULL;
static pthread_mutex_t annot_list_mutex = PTHREAD_MUTEX_INITIALIZER;

/* the following conditional variable is used when there is no
   element in the annot_list. The accompaning mutex will be
   annot_list_mutex. */
static pthread_cond_t annot_list_cond = PTHREAD_COND_INITIALIZER;

/* the following mutex is to prevent the overlapped access to the
   folding cash binary tree */
static pthread_mutex_t binary_tree_mutex = PTHREAD_MUTEX_INITIALIZER;

/* The following mutex it so prevent the main-thread from
   returning to main function  */
static pthread_mutex_t subthread_annotating_mutex = PTHREAD_MUTEX_INITIALIZER;

/* 
   This makes a plane of mutex. The coordinate of mutex plane
   corresponds to the cell of the world in the same
   coordinate. When one thread accesses to the cell, the
   corresponding mutex must be locked.
 */
static MutexPlane **mutexPlaneInitP()
{
  MutexPlane **a;
  a = (MutexPlane **) calloc(nrow+2,sizeof(MutexPlane *));
  if (a == NULL) {
    fprintf(stderr,"mutexPlaneInitP(): error in memory allocation\n");
    exit(1);
  }
  return a;
}

/* see mutexPlaneInitP() */
static MutexPlane **mutexPlaneInit()
{
  MutexPlane **a;
  int i,j;
  a = mutexPlaneInitP();
  a[0] = (MutexPlane *)calloc((nrow+2)*(ncol+2),sizeof(MutexPlane));
  if (a[0] == NULL) {
    fprintf(stderr,"mutexPlaneInit():error in memory allocation\n");
    exit(1);
  }
  for (i=1,j=nrow+2; i < j; i++)
    a[i] = a[i-1] + ncol + 2;

  return a;
}

/* It adds the new_pos at the end of the annotlist. It returns a
   pointer to the new element. On annotList, see paralell.h */
static AnnotList *annotListPush(struct point new_pos)
{
  AnnotList *new_list;

  /* make a new element of list */
  new_list = (AnnotList*) malloc(sizeof(AnnotList));
  if(new_list==NULL){
    fprintf(stderr,"annotListPush(): error in memory allocation\n");
    exit(1);
  }

  new_list->pos = new_pos;
  new_list->next = NULL; //because this element will be the tail

  /* if there is any element in the list */
  if(annot_list_tail!=NULL){
    /* connect this new element to the list */
    annot_list_tail->next = new_list;
    new_list->prev = annot_list_tail;

    /* move the pointer to the tail of the list */
    annot_list_tail = new_list;
  }
  /* if there is no element in the list */
  else{
    /* the new element is the head and the tail of the list */
    annot_list_head = new_list;
    annot_list_tail = new_list;
    new_list->prev = NULL;
  }
  
  return new_list;
}

/* It will put the value of the head of the annotlist in the
   given argument, and delete that element from the annotlist. If
   is is successful, returns 0. */
static int annotListPop(struct point *get)
{
  if(annot_list_head==NULL){
    fprintf(stderr,"annotListPop(): try to pop empty list\n");
    return 1;
  }

  *get = annot_list_head->pos;
  
  /* if head!=tail, there are more than one element */
  if(annot_list_head!=annot_list_tail){
    annot_list_head = annot_list_head->next;
    free(annot_list_head->prev);
    annot_list_head->prev = NULL;
  }
  /* if head==tail, there is only one element */
  else{
    free(annot_list_head);
    annot_list_head = NULL;
    annot_list_tail = NULL;
  }
  return 0;
}

/* It will delete the given element from the list */
static void annotListDelete(AnnotList *delete)
{
  if(delete==NULL){
    fprintf(stderr,"annotListDelete(): try to delete NULL\n");
    exit(1);
  }

  /* if this is not the head of list */
  if(delete!=annot_list_head){
    /* Note delete->next could be NULL */
    (delete->prev)->next = delete->next;
  }
  /* if this is the head of list */
  else{
    /* Note delete->next could be NULL */
    annot_list_head = delete->next;
  }

  /* if this is not the tail of list */
  if(delete!=annot_list_tail){
    /* Note delete->prev could be NULL */
    (delete->next)->prev = delete->prev;
  }else{
    /* Note delete->prev could be NULL */
    annot_list_tail = delete->prev;
  }

  free(delete);
  delete = NULL;
}



/* Initilizer of mutex plane. It allocates memory to the mutex
   plane by mutexPlaneInit() and initilize them by
   pthread_mutex_init(). Furthermore, it inilize the flag of
   mutex_plane. */
static void mutexInit(void)
{
  int i,j;
  mutex_plane = mutexPlaneInit();
  
  for(i=1;i<=nrow;i++)
    for(j=1;j<=ncol;j++){
      pthread_mutex_init(&(mutex_plane[i][j].mutex),NULL);
      mutex_plane[i][j].annotlist=NULL;
    }
}


/* This is the calling fanction of the sub-thread. It will get a
   cell to annotate from annotList. If there is nothing to
   annotate, it will wait. It will wake up by signal from the
   main thread, and the main thread will signal when it inserts a
   new task into the annotList. */
static void *annotByThread(void *world_void)
{
  struct point cur_point;
  TYPE2 **world = (TYPE2 **) world_void;
  
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  
  while(1){
    
    /* first, we lock the list */
    //printf("Sub: try to lock the list\n");
    pthread_mutex_lock(&annot_list_mutex);
    //printf("Sub: Done\n\n");
    
    /* check if the annot_list is empty */
    if(annot_list_tail!=NULL){
      /* featch a cell to update */
      annotListPop(&cur_point);
      
      /* we lock the following to denote that we are annotating a
	 cell, so that the main-thread can wait returning to main
	 function until we finish the annotation. */
      pthread_mutex_lock(&subthread_annotating_mutex);
      
      /* Then, we unlock the list */
      //printf("Sub: unlock the list\n");
      pthread_mutex_unlock(&annot_list_mutex);
      
      /* we must lock the cell before unlocking the task
	 list. Otherwise, the main thread may access the current
	 cell */
      //printf("\nSub: try to lock the cell[%d][%d]\n",cur_point.row,cur_point.col);
      pthread_mutex_lock(&mutex_plane[cur_point.row][cur_point.col].mutex);
      //printf("Sub: Done\n");

      /* Annotate molecules */
      //printf("Sub: RNAFoldpipe is folding\n");
      if(mutex_plane[cur_point.row][cur_point.col].annotlist!=NULL){
	AnnotMolCashByThread(&world[cur_point.row][cur_point.col],RNAFoldPipe,Ptype2kcatPipe);
	
	/* we put a flag to indicate that this cell is updated */
	mutex_plane[cur_point.row][cur_point.col].annotlist = NULL;
      }

      /* Before leaving this cell, we unlock the cell. */
      //printf("Sub: unlock the cell[%d][%d]\n",cur_point.row,cur_point.col);
      pthread_mutex_unlock(&mutex_plane[cur_point.row][cur_point.col].mutex);

      /* We let the main-thread return to main function */
      pthread_mutex_unlock(&subthread_annotating_mutex);

    }else{
      /* because there is no element in the list, we sleep until
	 been waken up by the main thread */
      //printf("Sub: no task left, sleep\n");
      while(annot_list_tail==NULL)
	pthread_cond_wait(&annot_list_cond,&annot_list_mutex);
      
      /* Now that we have lock of list_mutex, we unlock it. */
      //printf("Sub: got a task. Unlock the list\n");
      pthread_mutex_unlock(&annot_list_mutex);
    }
  }
  return NULL;
}

/* Initlizer of parallel process. It will start a RNAfold process
   that will be used by sub-thread. It will initilize mutex
   plane. */
void parallelInit(TYPE2 **world,char *target_structure)
{
  RNAOpen(target_structure);
  fprintf(stderr,"parallelInit(): an RNAfold process is opened.\n");
  mutexInit();
  
  if(0!=pthread_create(&thread_id,NULL,annotByThread,(void *)world)){
    fprintf(stderr,"parallelInit(): error in creating thread.\n");
    exit(1);
  }
  fprintf(stderr,"parallelInit(): a sub-thread is created\n");
  fflush(stderr);
}

void parallelClean(void)
{
  if(0!=pthread_cancel(thread_id))
    fprintf(stderr,"parallelClean(): failed to cancel the sub-thread.\n");
  else
    fprintf(stderr,"parallelClean(): the sub-thread is cancelled.\n");
  
  if(0!=pthread_join(thread_id,NULL))
    fprintf(stderr,"parallelClean(): failed to join with the sub-thread.\n");
  else
    fprintf(stderr,"parallelClean(): the sub-thread joined.\n");
  
  if(0!=RNAClose())
    fprintf(stderr,"parallelClean(): failed to close RNAfold process.\n");
  else
    fprintf(stderr,"parallelClean(): the RNAfold process is terminated.\n");
  
  fflush(stderr);
}

/* 
   This is the parallel version of MoleculeAction(). Before using
   this, parallelInit() must be called once.
*/
void parallelMoleculeAction(TYPE2 **world,struct updorder* updorder_p,struct point** neighbor[],double mutation)
{
  int i,irow,icol,irownei,icolnei;
  TYPE2 *icel,*icelnei;
  double kdiss3,kdiss5,kasso3,kasso5;
  int length = nrow*ncol;
  double temp;
  int nei; /* index of rownei and colnei */
  /* 516
     203
     748  */
  
  struct point pos;
  
  length = nrow*ncol;

  for(i=0;i<length;i++){
    irow=(updorder_p+i)->row;
    icol=(updorder_p+i)->col;
    /* chose a rondam neighbor */
    /* (the fractional part is truncated) */
    nei = 8.0*genrand_real3()+1; 
    irownei=neighbor[nei][irow][icol].row;
    icolnei=neighbor[nei][irow][icol].col;
      
    icel=&world[irow][icol];

    /* Before accessing the icelnei, we lock the list and the
       icelnei. When we do something to the CA plane, we always
       lock the list. This is crucial to prevent two threads
       accesing the same cell. The reason that we lock icelnei is
       that sub-thread might be annotating icelnei right now,
       then we have to wait till it finishes. */
    //printf("\nMain: try to lock the list\n");
    // pthread_mutex_lock(&annot_list_mutex);
    //printf("Main: Done\n");
    
    /* We must lock 'icelnei' because the sub-thread may be
       annotating 'icelnei' right now. In this case, we wait
       until the sub-thread finishes the annotation. */
    //printf("\nMain: try to lock the cell[%d][%d]\n",irownei,icolnei);
    pthread_mutex_lock(&mutex_plane[irownei][icolnei].mutex);
    //printf("Main: Done\n");

    /* if sub-thread is expected to annotate this, i.e., there is
       an entory in the 'annot_list' to update 'icelnei', we
       delete this enetry from the list. And, we annotate it by
       ourselves. The flag to check if 'icelnei' is exptected to
       be annotated is 'annolist' attributes. */
    if(mutex_plane[irownei][icolnei].annotlist!=NULL){
      /* we delete this cell from the task list (annotlist) so
	 that sub-thread will not annotate this anymore. */
      //annotListDelete(mutex_plane[irownei][icolnei].annotlist);
      
      /* we unlock the task list so that sub-thread can go on
	 annotating the other cells. But the list does not
	 contain 'icelnei' anymore, so the sub-thread will not
	 touch 'icelnei'. */
      //printf("Main: unlock the list\n");
      //pthread_mutex_unlock(&annot_list_mutex);

      /* We annotate 'icelnei' */
      AnnotMolCashByThread(&world[irownei][icolnei],fold,Ptype2kcat);

      /* We annotated 'icelnei', so put a flag */
      mutex_plane[irownei][icolnei].annotlist=NULL;

      /* unlock icelnei because nobody is going to touch 'icelnei' */
      //printf("Main: unlock the cell[%d][%d]\n",irownei,icolnei);
      pthread_mutex_unlock(&mutex_plane[irownei][icolnei].mutex);
    }
    /* If icelnei is accessible (either annotated already or not
       updated at all) */
    else{
      //printf("Main: unlock the list\n");
      //pthread_mutex_unlock(&annot_list_mutex);
      //printf("Main: unlock the cell[%d][%d]\n",irownei,icolnei);
      pthread_mutex_unlock(&mutex_plane[irownei][icolnei].mutex);
    }

    /* Now we can use icelnei */
    icelnei=&world[irownei][icolnei];

    /******************************************
    * From here we does the molecule reaction *
    *******************************************/
        /* If "icel" and "icelnei" are not empty */
    if(icel->seq[0]!='N'){
      /* and both "icel" and "icelnei " are free */
      if(((icel->bonr==-1 && icelnei->seq[0]!='N'))&& icelnei->bonr==-1){
	AssoDissRate(icel->end3,icelnei->end5,&kasso3,&kdiss3);
	AssoDissRate(icelnei->end3,icel->end5,&kasso5,&kdiss5);
	if((temp=kasso3+kasso5)==0.)
	  continue;
	else if(genrand_real1() < temp){
	  icel->bonr = irownei;
	  icel->bonc = icolnei;
	  icelnei->bonr = irow;
	  icelnei->bonc = icol;
	  if(genrand_real1()<kasso3/temp){
	    icel->term = 3;
	    icelnei->term = 5;
	    icel->kdis = kdiss3;
	    icelnei->kdis = kdiss3;
	  }
	  else{
	    icel->term = 5;
	    icelnei->term = 3;
	    icel->kdis = kdiss5;
	    icelnei->kdis = kdiss5;
	  }
	}
      }
	
      /* "icel" is making complex now, complex can be
	 breaked. Note that icel's complex partner will never be
	 been annotated right now, because the newly born
	 molecule (thus must be annotated) will not have any bond
	 to another molecule.*/
      else if(icel->bonr != -1) {
	if(genrand_real1() < icel->kdis){
	  world[icel->bonr][icel->bonc].bonr = -1;
	  icel->bonr = -1;
	}
      }
      /* else
	 do nothing */

      /* We put a flag to tell that 'icel' is updated. */
      mutex_plane[irow][icol].annotlist = NULL;
    }

    /* If "icel" is empty and if "icelnei" is complex. Note again
       that 'icelnei's bounding partner will not be been
       annotated by the sub-thread. */
    else if(icelnei->seq[0]!='N'&&icelnei->bonr!=-1){
      /* if "icelnei" is the catalyst */
      if(icelnei->term == 3){
	if((temp=icelnei->kcat)==0.){
	  /* This means that a new molecule was not
	     created. Thus, the updating is done. */
	  mutex_plane[irow][icol].annotlist = NULL;
	}
	/* In this case, a new molecule was born. */
	else if(genrand_real1()<temp*world[icelnei->bonr][icelnei->bonc].krec){
	  /* If Replication()>0, mutation occured */
	  Replication(icel->seq,world[icelnei->bonr][icelnei->bonc].seq,mutation);

	  /* Don't forget this. A new molecule is not bound to
	     anything. */
	  icel->bonr = -1; 

	  /* The complex molecule breaks apart after the
	     replication */
	  world[icelnei->bonr][icelnei->bonc].bonr = -1;
	  icelnei->bonr = -1;

	  /* We insert this molecule in annotlist */
	  //printf("\nMain: to insert, try to lock the list\n");
	  pthread_mutex_lock(&annot_list_mutex);
	  //printf("Main: Done\n");
	  pos.row = irow; pos.col = icol; 
	  mutex_plane[irow][icol].annotlist = annotListPush(pos);

	  /* When the main thread push a task in the annotList, it
	     must signal so that the sub-thread can wake up. The
	     sub-thread will be sleeping when there is no element in
	     the annoList. */
	  //printf("Main: send signal\n");
	  pthread_cond_signal(&annot_list_cond);
	  //printf("Main: unlock the list\n");
	  pthread_mutex_unlock(&annot_list_mutex);
	}
	/* If it fails to make a new molecule, the updating is
	   done. Thus, we put a flag to indicate that. */
	else{
	  mutex_plane[irow][icol].annotlist = NULL;
	}
      }
      /* if "icelnei" is the template */
      else if((temp=world[icelnei->bonr][icelnei->bonc].kcat)==0.){
	/* This means that a new molecule was not created. Thus,
	     the updating is done. */
	mutex_plane[irow][icol].annotlist = NULL;
      }
      /* In this case, a new molecule was born. */
      else if(genrand_real1()<temp*icelnei->krec){
	Replication(icel->seq,icelnei->seq,mutation);

	/* Don't forget this. A new molecule is not bound to
	   anything. */
	icel->bonr = -1;

	/* The complex molecule breaks apart when they makes a
	   new molecule. */
	world[icelnei->bonr][icelnei->bonc].bonr = -1;
	icelnei->bonr = -1;
	
	/* We insert this molecule in annotlist */
	//printf("\nMain: to insert, try to lock the list\n");
	pthread_mutex_lock(&annot_list_mutex);
	//printf("Main:Done\n");
	pos.row = irow; pos.col = icol; 
	mutex_plane[irow][icol].annotlist = annotListPush(pos);
	
	/* When the main thread push a task in the annotList, it
	   must signal so that the sub-thread can wake up. The
	   sub-thread will be sleeping when there is no element in
	   the annoList. */
	//printf("Main: send signal\n");
	pthread_cond_signal(&annot_list_cond);
	//printf("Main: unlock the list\n");
	pthread_mutex_unlock(&annot_list_mutex);
      }
      /* If it fails to make a new molecule, the updating is
	 done. So we put a flag. */
      else{
	mutex_plane[irow][icol].annotlist = NULL;
      }
    }
    /*****************************************/
  }
  
  

  
  /* This roop is after updating all the cells of CA plane. If
     there are remaining molecules to be updated, we update then
     here */
  while(1){
    /* first, we lock the list */
    //printf("\nMain: to finish up, lock the list\n");
    pthread_mutex_lock(&annot_list_mutex);
    //printf("Main:Done\n");

    /* if there is a cell to annotate, then do it. */
    if(annot_list_tail!=NULL){
      /* featch a cell to update */
      annotListPop(&pos);
      /* then unlock the list so that sub-thread can also
	 annotate the molecules. */
      //printf("Main: unlock the list for finishing up\n");
      pthread_mutex_unlock(&annot_list_mutex);

      /* Annotate the molecule */
      AnnotMolCashByThread(&world[pos.row][pos.col],fold,Ptype2kcat);

      /* we put a flag to indicate that this cell is updated */
      mutex_plane[pos.row][pos.col].annotlist = NULL;

    }
    /* If there is not a cell to annotate */
    else{
      /* then unlock the list */
      //printf("Main: unlock the list for finishing up\n");
      pthread_mutex_unlock(&annot_list_mutex);

      /* go out of the function. But before that, make sure that
	 there is no cell being annotated. To see that, we check
	 if sub-thread is working. 'subthread_annotating_mutex'
	 is locked by sub-thread before the list is unlocked by
	 sub-thread. Thus, after main-thread lock the list and
	 there is no cell to annotate, and if the sub-thread is
	 still annotating the last cell,
	 'subthread_annotating_mutex' is locked until the
	 sub-thread finishes his job. */
      pthread_mutex_lock(&subthread_annotating_mutex);
      pthread_mutex_unlock(&subthread_annotating_mutex);
      break;
    }
  }
  
}

/* This function does the same thing as AnnotMol() does. But with
   cashing for RNA folding.
   
   CAUTION, Do not change or initlize variable attributes of
   TYPE2, e.g. ".bonr", ".bonc" because these can have special
   meaning which should not modified by AnnotMolCash().
*/
static void AnnotMolCashByThread(TYPE2 *mole,float (*fold_fnc)(char*,char*),double (*ptype2kcat)(char *))
{
  mole->energy=MyFoldThread(mole->seq,mole->str,fold_fnc);
  //printf("%p: %s\n",fold_fnc,mole->str);
  /* if you don't want to use the extra freedom for parasites,
     make mol->krec=1. */
  mole->krec = 1.;
  
  openEnd(mole);
  //printf("%p: openEnd() is done\n",fold_fnc);
  mole->kcat = (*ptype2kcat)(mole->str);
  //printf("%p: ptype2kcat() is done\n",fold_fnc);
  mole->type = ColorByFunctionThread(mole,fold_fnc,ptype2kcat);
  //printf("%p: ColorByFunctionThread() is done\n",fold_fnc);
  //mole->type = ColorByGenotype(mole->seq);
}

double Ptype2kcatPipe(char *structure)
{
  if(RNADistancePipe(structure)==0)
    return kCatG;
  else
    return 0.;
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
int ColorByFunctionThread(TYPE2* mol,float (*fold_fnc)(char*,char*),double (*ptype2kcat)(char *))
{
  TYPE2 cmol;
  PerfectReplication(cmol.seq,mol->seq);
  (*fold_fnc)(cmol.seq,cmol.str);
  openEnd(&cmol);
  /* In the following, we do not use kthreashold. */
  if((*ptype2kcat)(cmol.str)> 0. /*this should be kthreashold */&& maxbasepair(cmol.end3)>mthreshold){
    if(mol->kcat> 0. && maxbasepair(mol->end3)>mthreshold)
      return 1;
    else
      return 3;
  }
  else
    if(mol->kcat> 0. && maxbasepair(mol->end3)>mthreshold)
      return 2;
    else
      return 4;
}

static double MyFoldThread(char *seq,char *str,float (*fold_fnc)(char*,char*))
{
  statusEnum status;
  double energy;
  nodeType *result;

  /* First, we lock the binary tree */
  pthread_mutex_lock(&binary_tree_mutex);

  /* If we can find this sequence in the cash */
  if((status=binFindP(seq,&result))==STATUS_OK){
    /* Give the result and return the energy */
    strcpy(str,result->shape);
    /* Unlock the binary tree immediately. */
    pthread_mutex_unlock(&binary_tree_mutex);
    return result->energy;
  }
  /* If we cannot find this sequence in the cash */
  else if (status==STATUS_KEY_NOT_FOUND){
    /* Unlock the binary tree immediately. */
    pthread_mutex_unlock(&binary_tree_mutex);

    /* We fold the sequence by the library (sub-thread will use
       pipe because the library may use global variables. ) */
    energy=(double)(*fold_fnc)(seq,str);

    /* lock the binary tree to insert a new sequence. */
    pthread_mutex_lock(&binary_tree_mutex);
    /*insert this seq in the binary tree genotype data base*/
    if(STATUS_MEM_EXHAUSTED==binInsertP(seq,&result)){
      fprintf(stderr,"MyFoldMainThread(): binInsertP() reported memory exhaustion.\n");
      exit(1);
    }
    strcpy(result->shape,str);
    result->energy=energy;
    /* Unlock the binary tree */
    pthread_mutex_unlock(&binary_tree_mutex);

    return energy;
  }
  /* This is error. */
  else
    return 1.;
}

