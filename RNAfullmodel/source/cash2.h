#include "constants.h"

#ifndef CASH2
#define CASH2

/* CA's cell type. You define your own type here. structure is
   allowed */
typedef struct __type2 { 
  /* permanent data */
  char seq[MAXLEN]; /* RNA sequence 3' -> 5' */
  char str[MAXLEN]; /* Sequence's 2ndary structure */
  char cseq[MAXLEN];
  char cstr[MAXLEN]; /* 2ndary structure of complementary
			sequence. */
  char end3[MAXLEN]; /* dangling end of 3' terminal 3'->5' */
  char end5[MAXLEN]; /* dangling end of 5' terminal 5'->3' */
  double kcat; /*  probability of polymerizing (intensity of
		   giving catalysis).  */
  
  char cend3[MAXLEN];
  char cend5[MAXLEN];
  double ckcat;
  
  double krec; /*  probability of being polymerized (intencity of
		   getting catalysis). This is used in
		   extra-freedom simulations. */
  double energy; /* the energy */
  double cenergy; /* the energy of complementary sequence */
  double kmut; /*if replicase, prob of per base mutation*/
  
  char who;    /*one character-functional description of the molecule*/
  
  /* variable data */
  int bonr;/* If the molecule is making a bond with its
	       neighboring molecule, then "bond" specifies the
	       cooridnate of a molecule with which the focal
	       molecule makes bond. If bonr = -1 then it does not
	       have a bond  */
  int bonc; /* "bonc" will have a alternative function in special
	       cases. See bin2Topn. */
  int term; /* it saids which terminal of it makes a bond. the
		   molecule which makes a complex with 3' end can
		   execute its catalytic activity. One with 5'
		   end will be a template. */
  double kdis; /* the prob. of dissociation when a molecule is
		  making complex */
  int type; /* catalyst=0; parasite=1 */
  char sign; /*+ or -, at initialization catalysts are +, complem seq are - */
  int anc[PAR_ANCESTOR]; /*This is for ancestor tracking: every par_backup timesteps 
                      everyone is given number, so by following the numbers it's 
                      possible to see who is the grand-grand-...-child of who*/ 
} TYPE2;


/* This structure is used in asynchronous updating to determine
   the order of the cells to update. Look at the functions in
   chash2.c which use "struct updorder" for details and the usage
   of this structure. Look at UpdOrdReset() in cash2.c  */
struct updorder { 
  int row;
  int col;
};

/* This structure is used in neighborhoods retrieving. Look
   at NeighSet() in cash2.c */
struct point { 
  int row;
  int col;
};

/* This structure is used in Margolus neighborhood
   retrieving. Read MarGolusNeigh(). If you say "MARGOLUS
   something_you_defined[*][i][j]" where [*] denote even-or-odd
   phase, then "----[*][i][j].m[CW].row" and
   "----[*][i][j].m[CW].col" give you the coordinate of the CW
   neighbor (see below) of Margolus neighborhood of the [i][j]
   cell in the phase [*] (even/odd, 0 or 1).

   -----------
   |HERE| CW |
   -----------
   | CCW| OPP|
   -----------

   Note that the exact place of "HERE" does not matter in that
   "CW", "OPP" and "CCW" notations are invariant for the position
   of "HERE".

   What is even and what is odd? Let "SYD" denote
   "something_you_defined". Then, SYD[0] and SYD[1] gives you tow
   alternative Margolus partitioning of the plane. Then, SYD[0]'s
   partition will be such that [0][0] cell of your plane will be
   the upper-left corner (UL) of the 2x2 square. SYD[1]'s
   partition will be such that [0][0] cell  of your plane will be
   the lower-right corner (LR) of the 2x2 square. See the
   followings too. 

   UL,UR,LL,LR are like,
   -----------
   | UL | UR |
   -----------
   | LL | LR |
   -----------


   in even phase (SYD[0]), the plane is divided as follows:
   -----------------
   | 1,1 | 1,2 | etc.
   -------------
   | 2,1 | 2,2 | etc.
   -------------
   | etc. etc.   etc.

   in odd phase (SYD[1]) and boundary=WRAP, the plane is divided as follows:
   ------------------
   |nr,nc| nr,1| etc.
   -------------
   | 1,nc|  1,1| etc.
   -------------
   | etc. etc.   etc.

   where "nr" is the number of row, and "nc" is the number of
   column. If boundary==FIXED, then read the avobe table such
   that "nr=nc=0".
*/
#define CW (0)
#define OPP (1)
#define CCW (2)
typedef struct __margolus{
  struct point m[3];/* 0:clockwise 1:opposite 2:counter-clockwise */
} MARGOLUS;


/*********************************************basic*/
TYPE2 **NewP2(void);
TYPE2 **New2(void);
int PlaneFree2(TYPE2**);
void UpdOrdReset(struct updorder**);
TYPE2 **Copy2(TYPE2 **,TYPE2 **);
TYPE2 **Fill2(TYPE2 **,TYPE2);

/*********************************************shift*/
TYPE2 **Boundaries2(TYPE2**);

/*****************************************neighbors*/
struct point **NeighSet(int); 
void NeighFree(struct point**);

/******************************************margolus*/
MARGOLUS ***MargolusNeigh(void);
void MargolusFree(MARGOLUS***);
void MargolusDiffusion(TYPE2**,MARGOLUS***);

#endif //CASH2
