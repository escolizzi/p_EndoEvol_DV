/*This is the prototype of the prototype: in this version
all the features I added and worked are gathered.
(such as 'who', anc-trace, blobby colouring...)
Also, it should be a little bit better organized.
(And even some spelling mistakes from 
the sensei (god forbids!) are corrected)*/

/*Version 0.05: World size big again (512x512); output divided into + and - 
strings (pmutatis and mmutatis), mutation step 3 times smaller then prev version
...

Version 0.02: I make mutation rate local, mut rate mutates in time 
only if child sequence is mutated from father. Smaller grid.
Store data of mut rate to file once in "timestep" (see MoleculeAction in this file). 
No display made while running (see para.c).
All the rest is left the same*/

/* Yet Prototype */

/*
Version 8.2: I put MoleculeActionNull(). Distance must be d<5 to
replciate. Later I changed this number to a parameter in para.c
file. I set it to d<=3.

Version 8.1: I want to see the evolutionary dynamics without
parasites (but only with junc molecules). I device another
algorithm for molecular reaction. ReadWorldPartParasite(),
MoleculeActionParasite(), AnnotMolCashParasite(),
AnnotMolCashNoMutationParasite() are newly made. Defining the
macro PARASITE_NOT_REPLICATED will turn on this feature.

Version 8 I try to make it like Object Oritented.

Version 7.01 To follow the linage of general parasites, I
inserted a code in MoleculeAction() to copy the color of a parent
to the color of child if the parent is generalist parasite. I
also inserted a code in ReadWorldPart() to put a special color in
the general parasite. 

Version 7.0 I made MyFold() effecient by reducing the call of
BinFindP(). I made replication without mutation faster by adding
the structure and the energy of complementary string to TYPE2. I
made MoleculeActionClaim().

Version 6.0 under const. paralerize. Failed to speed up. Discarded.

Version 5.0 Add the energy depenent replication.

Version 4.11 Output backup files in the backup directory. Takes
-Mut, -MovieInt and -BackupInt arguments.

Version 4.1 Add non-graphical run mode (par_if_display=0). Add signal
finish mode (send USR1, it will make a buchup of the plane data,
and will stop the simulation).

Version 4.0 Add phenotype tracking.

Version 3.11 Bugs in MargolusDiffusion() is fixed, I hope.

Version 3.1 Change the cashing method and error handling.

Version 3.0 Add implicit modeling of RNA molecules: the complex
molecules formation is not modeled. MargolusDiffusion() is added.

Version 2.0 Add explicit modeling of RNA molecules: The molecules
make complex and a complex can break into two
molecules. Diffusion is random walk.

Version 1.1 I changed neighborhoods retrival so that WRAP and ECHO can be
used as boundary condition. And, I added GetInput() function so that User
can supply parameters from stdin.

Version 1.0 The program simulates replication process of S-molecule in
subject to the catalysis of of E-molecule. S and E can bind each other and
make a complex-SE. A complex-SE can give rise to S-molecules. I created
this program to verify whether the general model formalism can correctly
simulate titlation process.
*/


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <grace_np.h>
#include <unistd.h>
#include <fold.h>
#include <RNAstruct.h>
#include <dist_vars.h>
#include <treedist.h>
#include <float.h>
#include <limits.h>
#include <signal.h>

#include "constants.h"
#include "cash.h"
#include "cash2.h"
#include "para.h"

#include "mersenne.h"
#include "phenotree.h"
#include "my.h"
#include "rna_string.h"
#include "fold_wrapper_lib.h"
#include "annotmol.h"

#include "observer_wrapper.h"


TYPE2 global_empty={"N","\0","\0","\0",		/*permanent data*/
		    "\0","\0",0.,
		    "\0","\0",0.,
		    0.,0.,0.,-1.,
		    'N',			/*who*/
		    -1,0,0,0.,-1,'N',		/*variable data*/
		    {0}};			/*ancestor trace*/


/* etc. parameters */
static int poseG = 0; /* if 1, gotMouse() will be invoked */
static int stopG=0; /* if it is 1, simulation stops */
static int signalStopG=0; /* if it is 1, simulation save and stops, but not as an error */
static int nNodeMax;

/* debugging etc. */
int ncalls_myfold=0;
int nsucc_foldcash=0;


/******************************************/
/* basic declaration irrelevant of model  */
/******************************************/
extern int nrow,ncol,scale,boundary,boundaryvalue;
extern TYPE2 boundaryvalue2;

int main(int argc, char **argv)
{

  TYPE2 **world;
  struct updorder *updorder_p;
  struct point **neighbor[9];
  //  MARGOLUS ***margolus;
  int Time;
  int maxtime;
  int i;

  nrow = 512; /* This does not include boundary */
  ncol = 512;
  scale= 1;

  boundary=WRAP;
  boundaryvalue=0;
  boundaryvalue2.seq[0]='B'; /* Boundary. Do not change to N! */
  boundaryvalue2.bonr = -1;

  /* Calculate the interval with which we renwe tree. This is
     based on the approximate # of mutants produced in one time
     step. This is proportional to nrow*ncol*(1-q^len) where q is
     1-mutation_rate. */
  nNodeMax =  par_cash_size*1000.*1000./(double)sizeof(nodeType);


  /* Allocating memory to planes */
  world = New2();

  /* Arguments */
  Args(argc,argv);

  /* Initialize the planes and the boundary conditions and the random
     number generator and output file(s) and the window. */
  Initial(world);
  

  /* Initilize the observer */
  ObserverWrapper_init();

  /* Set neighbor plane. "boundary" and boundary value variables has to be
     set before. */
  for(i=0;i<9;i++)
    neighbor[i]=NeighSet(i);

  /* Set Margolus neighborhood */
  //  margolus = MargolusNeigh();

  /* Output header of the log */
  fprintf(stderr,"%s\nStarted at: %s\n",par_version,OutputTime());

  /******************/
  /*** MAIN CYCLE ***/
  /******************/
  for (Time = par_inittime,maxtime=par_maxtime; Time <= maxtime; Time++) {

    /*This makes movie at every step from time Time*/
    /*if(Time==500) par_movie_interval=1;
    */
    /* debug */
    ncalls_myfold=0;
    nsucc_foldcash=0;

    /* This also checks the state of population, 
    sets stopG and gives numbers to anc*/
    OutputAll(world,Time);
    if(Time % par_xmgrace_interval==0)
    /*AnnotPlane(world);*/	/*No need for this function, as we complement the whole information immediately*/
    ObserverWrapper_main(Time,(const TYPE2**)world, neighbor);

    if(stopG == 1)
      break;

    if(binGetNNode()>=nNodeMax)
      binRenew(world,nrow,ncol);
    
    /*MOVEMENT*/
    UpdOrdReset(&updorder_p);
    RandWalk(world,updorder_p,neighbor);
    
    /*MOLECULAR DYNAMICS*/
    UpdOrdReset(&updorder_p);
    MoleculeAction(world,updorder_p,neighbor,Time);
    MoleculeDecay(world);
    
    if(par_if_display==1)
      if(Mouse()==1||poseG==1) (void)gotMouse(world,Time);

    if(signalStopG==1){
      SignalStop(world,Time);
      break;
    }

    /* debug */
    //    printf("ncalls_myfold= %d nsucc_foldcash= %d\n",ncalls_myfold,nsucc_foldcash);
  }

  fprintf(stderr,"%s\nEnded (debug) at: %s\n",par_version,OutputTime());

  /* output time. Close window. Close outfile. Remove endfile. */
  Finish(world,neighbor,updorder_p);

  if(signalStopG==1)
    fprintf(stderr,"Signal Finished at:\n%s\n",OutputTime());
  else
    fprintf(stderr,"Normally Finished at: %s\n",OutputTime());
  
  return (0);
}



/*******************
 * Initialization  *
 *******************/
void Initial(TYPE2 **world)
{
  int r=0,g=255,b=255, c=0;	//amount of each color RGB, c is counter
  double nr=250.;    //nr of ColorRGB's you want to create
  double range=1000.;  // range of coloursteps
  double x = range/nr;           
  int y=0;
    
  
  /* Set signal handler */
  if(SIG_ERR == signal(SIGUSR1,SigCatch)){
    fprintf(stderr,"Initial:failed to set signalhandler.\n");
    exit(1);
  }

  init_genrand(par_ulseed);
  //initialize_fold(MAXLEN,NULL);

  /* Initilize the field */
  if(strlen(par_input_file_name)==0){
    testworld(world);
  }
  /* read from the file */
  else{
    ReadWorldPart(world);
  }

  Boundaries2(world); /* "world" should be done first because "view" is -> */

  /* Initilize xmgrace. If par_if_display==1, xmgrace; otherwise
     grace_batch */
  //InitXmgrace();

  /* Initilize colors for movies and graphics */
  if(par_if_movie==1 || par_if_display==1){
    ColorRGB(250,0,0,0);
    ColorRGB(251,255,255,255);
    ColorRGB(252,102,51,102);
    ColorRGB(253,153,51,102);
    ColorRGB(254,102,204,204);
    ColorRGB(255,0,204,153);
    ColorRGB(256,0,204,51);
    ColorRGB(257,153,0,51);
    ColorRGB(258,255,0,153);
    ColorRGB(259,0,51,0);
    ColorRGB(260,204,255,204);
    ColorRGB(261,102,153,153);
    ColorRGB(262,153,51,153);
    ColorRGB(263,255,255,153);
    ColorRGB(264,255,0,255);
    ColorRGB(265,51,102,0);
    ColorRGB(266,0,102,204);
    ColorRGB(267,51,0,204);
    ColorRGB(268,51,51,51);
    ColorRGB(269,51,0,0);
    ColorRGB(270,204,0,51);
    ColorRGB(271,153,0,204);
    ColorRGB(272,255,51,0);
    ColorRGB(273,153,0,0);
    ColorRGB(274,255,153,153);
    ColorRGB(275,0,204,255);
    ColorRGB(276,204,0,102);
    ColorRGB(277,102,255,153);
    ColorRGB(278,0,51,204);
    ColorRGB(279,102,255,51);
    ColorRGB(280,153,102,51);
    ColorRGB(281,255,102,0);
    ColorRGB(282,204,51,102);
    ColorRGB(283,102,255,102);
    ColorRGB(284,153,102,0);
    ColorRGB(285,0,204,204);
    ColorRGB(286,102,204,153);
    ColorRGB(287,204,102,102);
    ColorRGB(288,102,153,255);
    ColorRGB(289,255,153,102);
    ColorRGB(290,204,255,0);
    ColorRGB(291,102,102,204);
    ColorRGB(292,204,153,153);
    ColorRGB(293,51,0,153);
    ColorRGB(294,102,153,102);
    ColorRGB(295,255,204,204);
    ColorRGB(296,255,255,51);
    ColorRGB(297,0,102,153);
    ColorRGB(298,51,102,102);
    ColorRGB(299,204,153,204);
    ColorRGB(300,0,102,255);
    ColorRGB(301,102,51,204);
    ColorRGB(302,51,153,204);
    ColorRGB(303,102,153,204);
    ColorRGB(304,0,102,102);
    ColorRGB(305,255,255,204);
    ColorRGB(306,102,51,51);
    ColorRGB(307,51,255,102);
    ColorRGB(308,102,102,51);
    ColorRGB(309,153,153,153);
    ColorRGB(310,204,102,51);
    ColorRGB(311,204,51,153);
    ColorRGB(312,204,51,51);
    ColorRGB(313,153,153,102);
    ColorRGB(314,51,51,204);
    ColorRGB(315,0,153,153);
    ColorRGB(316,153,0,255);
    ColorRGB(317,204,0,204);
    ColorRGB(318,51,153,51);
    ColorRGB(319,51,102,255);
    ColorRGB(320,102,0,153);
    ColorRGB(321,0,102,0);
    ColorRGB(322,0,51,153);
    ColorRGB(323,102,51,153);
    ColorRGB(324,204,255,102);
    ColorRGB(325,153,51,0);
    ColorRGB(326,102,153,0);
    ColorRGB(327,153,153,204);
    ColorRGB(328,102,204,0);
    ColorRGB(329,0,102,51);
    ColorRGB(330,204,153,51);
    ColorRGB(331,51,51,0);
    ColorRGB(332,255,204,51);
    ColorRGB(333,204,102,0);
    ColorRGB(334,255,51,153);
    ColorRGB(335,255,153,0);
    ColorRGB(336,153,204,0);
    ColorRGB(337,204,204,51);
    ColorRGB(338,0,51,102);
    ColorRGB(339,51,153,102);
    ColorRGB(340,51,0,51);
    ColorRGB(341,102,204,102);
    ColorRGB(342,153,102,102);
    ColorRGB(343,204,102,204);
    ColorRGB(344,255,204,102);
    ColorRGB(345,0,0,255);
    ColorRGB(346,102,0,51);
    ColorRGB(347,204,255,153);
    ColorRGB(348,153,204,255);
    ColorRGB(349,153,51,255);
    ColorRGB(350,255,51,204);
    ColorRGB(351,102,0,0);
    ColorRGB(352,51,255,153);
    ColorRGB(353,255,204,153);
    ColorRGB(354,153,153,51);
    ColorRGB(355,204,51,204);
    ColorRGB(356,51,102,153);
    ColorRGB(357,51,0,102);
    ColorRGB(358,51,204,0);
    ColorRGB(359,204,51,255);
    ColorRGB(360,102,51,255);
    ColorRGB(361,153,204,204);
    ColorRGB(362,204,204,102);
    ColorRGB(363,255,153,204);
    ColorRGB(364,102,255,204);
    ColorRGB(365,0,0,153);
    ColorRGB(366,102,102,255);
    ColorRGB(367,0,51,51);
    ColorRGB(368,255,0,102);
    ColorRGB(369,204,0,0);
    ColorRGB(370,0,153,255);
    ColorRGB(371,0,153,102);
    ColorRGB(372,51,51,153);
    ColorRGB(373,153,153,0);
    ColorRGB(374,51,204,102);
    ColorRGB(375,255,102,255);
    ColorRGB(376,51,255,204);
    ColorRGB(377,153,102,255);
    ColorRGB(378,204,255,51);
    ColorRGB(379,51,204,255);
    ColorRGB(380,153,51,51);
    ColorRGB(381,255,102,51);
    ColorRGB(382,255,255,102);
    ColorRGB(383,102,204,255);
    ColorRGB(384,153,204,153);
    ColorRGB(385,255,204,0);
    ColorRGB(386,51,204,51);
    ColorRGB(387,0,0,102);
    ColorRGB(388,204,153,102);
    ColorRGB(389,0,255,255);
    ColorRGB(390,153,153,255);
    ColorRGB(391,102,102,102);
    ColorRGB(392,153,0,102);
    ColorRGB(393,255,153,51);
    ColorRGB(394,255,51,51);
    ColorRGB(395,204,204,0);
    ColorRGB(396,51,204,153);
    ColorRGB(397,0,0,204);
    ColorRGB(398,0,255,0);
    ColorRGB(399,0,255,102);
    ColorRGB(400,153,102,153);
    ColorRGB(401,204,204,153);
    ColorRGB(402,204,153,0);
    ColorRGB(403,204,0,153);
    ColorRGB(404,153,51,204);
    ColorRGB(405,51,51,102);
    ColorRGB(406,0,0,51);
    ColorRGB(407,0,153,51);
    ColorRGB(408,102,255,0);
    ColorRGB(409,102,204,51);
    ColorRGB(410,204,204,204);
    ColorRGB(411,255,102,153);
    ColorRGB(412,0,153,0);
    ColorRGB(413,102,0,255);
    ColorRGB(414,204,255,255);
    ColorRGB(415,153,102,204);
    ColorRGB(416,255,102,102);
    ColorRGB(417,51,0,255);
    ColorRGB(418,51,102,51);
    ColorRGB(419,0,255,153);
    ColorRGB(420,255,0,204);
    ColorRGB(421,255,102,204);
    ColorRGB(422,204,51,0);
    ColorRGB(423,51,153,255);
    ColorRGB(424,0,153,204);
    ColorRGB(425,51,153,0);
    ColorRGB(426,102,0,204);
    ColorRGB(427,51,102,204);
    ColorRGB(428,51,153,153);
    ColorRGB(429,255,0,0);
    ColorRGB(430,0,255,51);
    ColorRGB(431,153,255,102);
    ColorRGB(432,153,204,102);
    ColorRGB(433,102,0,102);
    ColorRGB(434,51,204,204);
    ColorRGB(435,153,255,0);
    ColorRGB(436,255,153,255);
    ColorRGB(437,255,51,255);
    ColorRGB(438,204,0,255);
    ColorRGB(439,204,102,255);
    ColorRGB(440,204,204,255);
    ColorRGB(441,255,204,255);
    ColorRGB(442,0,51,255);
    ColorRGB(443,0,204,0);
    ColorRGB(444,204,153,255);
    ColorRGB(445,0,255,204);
    ColorRGB(446,153,0,153);
    ColorRGB(447,153,255,204);
    ColorRGB(448,153,255,255);
    ColorRGB(449,51,255,0);
    ColorRGB(450,102,102,0);
    ColorRGB(451,102,51,0);
    ColorRGB(452,153,255,153);
    ColorRGB(453,255,51,102);
    ColorRGB(454,102,255,255);
    ColorRGB(455,255,0,51);
    ColorRGB(456,102,153,51);
    ColorRGB(457,51,255,255);
    ColorRGB(458,51,255,51);
    ColorRGB(459,204,102,153);
    ColorRGB(460,153,255,51);
    ColorRGB(461,0,204,102);
    ColorRGB(462,153,204,51);
    ColorRGB(463,102,102,153);
    ColorRGB(464,255,255,0);
    ColorRGB(465,51,51,255);
    ColorRGB(466,128,128,128);
    
    for(c=0;c<(int)range;c++){		/*you start from cyan*/
      if(c<255){
        b = b - 1;	/*first go green*/
      }else if(c<255*2){
        r = r + 1;	/*then yellow*/
      }else if(c<255*3){
        g = g - 1;	/*then to red*/
      }
      else if(c<255*4)
        b= b +1;		/*then magenta*/
      if(c == (int)(y*x+0.5)){
        ColorRGB(1+y,r,g,b);
        y++;
      }
    }
  
  ColorRGB(0,0,0,0);	/*color number '0' has to be black!*/
  ColorRGB(466,128,128,128);	/*color if undef, dark grey*/
  }
}


void testworld(TYPE2 **world)
{
  /* Experiment for specified population */
  if(1){
    int i=0,j=0;
    TYPE2 init[12];
    TYPE2 empty;
    int nseq = 2;
    
    empty = global_empty;
    init[0] = empty;
    for(i=0;i<nseq*2;i++){
      init[i] = empty;
    }
    strcpy(init[0].seq,par_init_seq);
    strcpy(init[2].seq,par_c_init_seq);
    
    init[0].sign = '+';			/*initialize sign: "+" for catalist, "-" for complementary sequence*/
    init[2].sign = '-';
    
    for(i=0;i<=nseq;i+=2)
    {
      PerfectReplication(init[i+1].seq,init[i].seq);
      if (init[i].sign == '+') init[i+1].sign = '-';
      else if (init[i].sign == '-') init[i+1].sign = '+';
      else fprintf(stderr,"Error at initializing signs in function testworld");
    }


    for(i=0;i<nseq*2;i++){
      AnnotMolCash(init+i);
      init[i].bonr = -1;
      init[i].bonc = -1;
      init[i].kmut = 0.000010;
      for(j=0;j<PAR_ANCESTOR;j++) init[i].anc[j]=0;
    }

    /* The following is to output sequence at Time=0 by
       p-observer. We need to complete the information about the
       completemtary sequence to output the sequence . */
    for(i=0;i<=(nseq-1)*2;i+=2){
      strcpy(init[i].cstr,init[i+1].str);
      strcpy(init[i+1].cstr,init[i].str);
    }

    for(i=1;i<=nrow;i++)
      for(j=1;j<=ncol;j++){
	world[i][j] = empty;
	
/*      if(i>230 && i<280){
        if(j>230 && j<280){
*/	  if(genrand_real1()<0.5) world[i][j]=init[0];
	  else world[i][j]=init[2];
/*	}
      }
*/      /*
	if(genrand_real1()<0.8){
	  if(genrand_real1()<0.5){
	    world[i][j]=init[0];
	  }
	  else{
	    world[i][j]=init[1];
	  }
	}
	
	if (j<ncol/2){
	  if(genrand_real1()<0.5){
	    world[i][j]=init[0];
	  }
	  else{
	    world[i][j]=init[1];
	  }
	}else{
	  if(genrand_real1()<0.5){
	    world[i][j]=init[2];
	  }
	  else{
	    world[i][j]=init[3];
	  }
	}*/
      }
    /* error check */
    if(world[i][j].cstr[0]!='\0'){
      fprintf(stderr,"testworld(): error in initial value of cstr\n");
      exit(-1);
    }
  }
}

/* 
   This function reads a file to initilize the plane. The size of
   the file can be larger or smaller than the actual plane
   size. When the file contains a larger space, the actual space
   will be a part that is cut out from the upper left corner of
   the file space. When the actual space is larger than the file
   space, then the file space will be placed in the upper left
   corner of the actual space, and the rest will be empty.
*/
void ReadWorldPart(TYPE2 ** world)
{
  FILE* fin;
  int nr,nc,frow,fcol;
  int i,j,k;
  char seq[MAXLEN];
  char str[MAXLEN];
  char kmut[BUFSIZE]; /*This is for alternative backup format*/
  TYPE2 empty;
  int length;
  nodeType *result;
  
  fin = fopen(par_input_file_name,"r");
  if(fin == NULL){
    fprintf(stderr,"ReadWorldPart: can't open %s\n",par_input_file_name);
    exit(1);
  }

  /* The idea of implimentation is the following: We go though
     two loops of row and column. The nubmer of row and column
     will be set to those of a larger space (file or real
     space). If we are within the valid space of file, we read a
     line; if not, then consider it as empty. If we are within
     the valid space of the world, we place a cell; if not, then
     discard the cell read from file (if any). */
  fgets(seq,MAXLEN,fin);
  strtok(seq," "); /* strip # */
  frow = atoi(strtok(NULL," ")); /* read # of row */
  strtok(NULL," "); /* strip X */
  fcol = atoi(strtok(NULL," \n")); /* read # of col */

  nr = (nrow>frow) ? nrow : frow;
  nc = (ncol>fcol) ? ncol : fcol;

  empty=global_empty;
  for(i=1;i<=nr;i++)
    for(j=1;j<=nc;j++){
      if(i<=frow && j<=fcol){
	if(fgets(seq,MAXLEN,fin)==NULL){
	  fprintf(stderr,"ReadWorldPart: error in the file\n");
	  exit(1);
	}
	else if(fgets(str,MAXLEN,fin)==NULL){
	  fprintf(stderr,"ReadWorldPart: error in the file\n");
	  exit(1);
	}
/*	else if(fgets(kmut,BUFSIZE,fin)==NULL){
	  fprintf(stderr,"ReadWorldPart: error in the file\n");
	  exit(1);
	}
*/	
	if(strlen(seq)>= MAXLEN || strlen(str)>= MAXLEN){
	  fprintf(stderr,"ReadWorldPart: sequence length is too large. Change MAXLEN\n");
	  exit(1);
	}
      }else{
	seq[0]='N';
      }

      if(i<=nrow && j<=ncol){
	/* Initilize a cell by empty. I do this because I want to
	   initialize all the attributes of the cell first. */
	world[i][j]=empty;
	
	/* if this is not an empty cell, we overwrite the cell. */
	if(seq[0]!='N'){
	  length = strlen(seq);
	  strncpy(world[i][j].seq,seq,length);
	  strncpy(world[i][j].str,str,length);

	  world[i][j].kmut = par_mutation;
	  /*world[i][j].kmut=atof(kmut);*/

	  /* "seq" and "str" contains '\n'. */
	  world[i][j].seq[length-1] = '\0';
	  world[i][j].str[length-1] = '\0';
	  
	  /* AnnotMolCash() cannot be used. Because it's
	     unefficient to fold the sequence again; futhermore, we
	     need to know not only the fold but also the energy
	     which is not in the input file. 
	     However we have to complement information to know the function*/
	  world[i][j].energy=(double)energy_of_structure(world[i][j].seq,world[i][j].str,0);
	  
	  world[i][j].krec = 1.;
	  openEnd(&world[i][j]);
	  world[i][j].kcat = Ptype2kcat(world[i][j].str);
	  world[i][j].bonr = -1;
	  world[i][j].sign = '+';
	  
	  PerfectReplication(world[i][j].cseq,world[i][j].seq);
	  world[i][j].cenergy=MyFold(world[i][j].cseq,world[i][j].cstr);
  	  world[i][j].ckcat=Ptype2kcat(world[i][j].cstr);
  	  openEnd(&world[i][j]);
  	  world[i][j].who=det_phen(world[i][j].seq,world[i][j].cseq,world[i][j].kcat, world[i][j].end5, world[i][j].end3, world[i][j].ckcat, world[i][j].cend5, world[i][j].cend3);
	  for(k=0;k<PAR_ANCESTOR;k++) world[i][j].anc[k]=0;
	  
	  /* put this molecule in the data base */
	  if(binInsertP(world[i][j].seq,&result)==STATUS_OK){
	    strcpy(result->shape,world[i][j].str);
	    result->energy=world[i][j].energy;
	  }
	}/*else{ 
	   If this is an empty cell, the cell has already been
	   initilized to empty above. } */
      }
    }
  fclose(fin);
}


/***************************************
 *         Molecular Dynamics          *
 ***************************************/
void MoleculeAction(TYPE2 **world,struct updorder* updorder_p,struct point** neighbor[],int Time)
{
  int i,j,irow,icol,irownei,icolnei;
  int length;
  TYPE2 *icel,*icelnei,*parent;
  //double kdiss3,kasso3;
  double kdiss3,kdiss5,kasso3,kasso5;
  double mutation;
  double temp;								/*to store data*/
  
  int nei; /* index of rownei and colnei */
  /* 516
     203
     748  */

  length = nrow*ncol;
  
  for(i=0;i<length;i++)
    {
      irow=(updorder_p+i)->row;
      icol=(updorder_p+i)->col;

      icel=&world[irow][icol];

      /* choose a random neighbor */
      /* (the fractional part is truncated) */
      nei = 8.0*genrand_real3()+1; 
      irownei=neighbor[nei][irow][icol].row;
      icolnei=neighbor[nei][irow][icol].col;
      
      icelnei=&world[irownei][icolnei];

      /* If "icelnei" is boundary, discard it. */
      if(icelnei->seq[0]=='B'){
	continue;
      }

      /* If "icel" and "icelnei" are not empty */
      if(icel->seq[0]!='N'){
	if ((Time%timestep) == 0)
	  { 							/*ROUGH!*/
	  if (ToFile(icel->sign, Time, icel->kmut, icel->kcat, icel->seq, icel->who, icel->anc) != 0) 
	    fprintf(stderr, "Error in storing info...no comment!");
	  }
	/* and both "icel" and "icelnei " are free */
	if(((icel->bonr==-1 && icelnei->seq[0]!='N'))&& icelnei->bonr==-1){
	  //the focal cell (icel) attempts to form complex with its 5' end
	  
		// Slow unbiased rules for complex formation
		// AssoDissRate(icel->end3,icelnei->end5,&kasso3,&kdiss3);   //Nobuto's mistake: what he calls 3' end is 5' end and other way around
	  // if(kasso3==0.) continue;
	  // if(genrand_real1()<kasso3){
	  //   icel->bonr = irownei; icel->bonc = icolnei; //set flags for complex
	  //   icelnei->bonr = irow; icelnei->bonc = icol;
		// 
	  //   icel->term = 3;                             //set flags for position in complex 
	  //   icelnei->term = 5;
	  //   icel->kdis = icelnei->kdis = kdiss3;
	  // }
	  
	  //fast rules for complex formation - makes its chances twice as likely in a montecarlo step - it's ok
	
	  AssoDissRate(icel->end3,icelnei->end5,&kasso3,&kdiss3);   //Nobuto's mistake: what he calls 3' end is 5' end and other way around
	  AssoDissRate(icelnei->end3,icel->end5,&kasso5,&kdiss5);
	  
	  if((temp=kasso3+kasso5)==0.)
	    continue;
	  else if(2.*genrand_real1() < temp){       //multipl. by 2 normalises Nobuto's complex formation rule... but doubles chances of complex formation
	    icel->bonr = irownei; icel->bonc = icolnei;
	    icelnei->bonr = irow; icelnei->bonc = icol;
	    if(genrand_real1()<kasso3/temp){
	      icel->term = 3;
	      icelnei->term = 5;
	      icel->kdis = icelnei->kdis = kdiss3;
	    }
	    else{
	      icel->term = 5;
	      icelnei->term = 3;
	      icel->kdis = icelnei->kdis = kdiss5;
	    }
	  }
	
	
	
	}
	
	/* "icel" is making complex now, complex can be broken */
	else if(icel->bonr != -1) {
	  if(genrand_real1() < icel->kdis){
	    world[icel->bonr][icel->bonc].bonr = -1;
	    icel->bonr = -1;
	  }
	}
	/* else
	   do nothing */
      }

      /* If "icel" is empty and if "icelnei" is complex*/
      else if(icelnei->seq[0]!='N'&&icelnei->bonr!=-1){
	/* if replication, 'parent' will get a pointer to the
	   parent molecule. */
	parent = NULL;
	/* if "icelnei" is the catalyst */
	if(icelnei->term == 3){
	  if((temp=icelnei->kcat)==0.)
	    continue;
	  else if(genrand_real1()<temp*world[icelnei->bonr][icelnei->bonc].krec){
	    mutation = icelnei->kmut;			/*if icelnei is catalist, mutation takes its kmut*/
	    parent = &world[icelnei->bonr][icelnei->bonc];
	  }
	}else{
	  if((temp=world[icelnei->bonr][icelnei->bonc].kcat)==0.)
	    continue;
	  else if(genrand_real1()<temp*icelnei->krec){
	    						/*if not, then [bonr,bonc] is the catalist*/
	    mutation = world[icelnei->bonr][icelnei->bonc].kmut;
	    parent = icelnei;
	  }
	}
/* if 'parent!=NULL', replication happens */
	if(parent!=NULL){
	  /* If Replication()>0, mutation occured */
	  if(0==Replication(icel->seq,parent->seq,par_mutation)){
	    icel->kmut = parent->kmut;
	    AnnotMolCashNoMutation(icel,parent);
	  }else{
	    icel->kmut = ChangeMut(parent->kmut);   //notice that this is not used, global parameter par_mutation is used instead
	    AnnotMolCash(icel);
	  }
	  if (parent->sign == '+') icel->sign = '-';
	  else if (parent->sign == '-') icel->sign = '+';
          for(j=0; j<50;j++) icel->anc[j]=parent->anc[j];
	  
	  icel->bonr = -1; /* Don't forget this */
	  /* The complex molecule breaks apart */
	  world[icelnei->bonr][icelnei->bonc].bonr = -1;
	  icelnei->bonr = -1;
	}
      }
    }
    if ((Time%timestep) == 0) printf("%d\n", Time);		/*Print time to screen*/
    
}


/*takes mutation rate from parent->kmut, adds a small numb 
  taken from unif distr [-0.0001,0.0001]. If newmut goes below 0, 
  the excedence will be flipped to positive (according to Nobuto 
  this should make the distribution uniform again).*/
double ChangeMut(double mut)
{
  double rand, mutatis, newmut;
  
  mutatis = mut;
  rand = (genrand_real1() - 0.5)/30000;
  if ((newmut=mutatis+rand)>=0) return newmut;
  else return ((-1)*newmut);
}


void MoleculeDecay(TYPE2 **world)
{
  int nr,nc;
  int i,j;
  double decay;
  TYPE2 empty;

  nr = nrow;
  nc = ncol;
  decay = par_kdec;
  empty=global_empty;


  for(i=1;i<=nr;i++)
    for(j=1;j<=nc;j++){
      if(world[i][j].seq[0]!='N'){
	if(genrand_real1()<decay){
	  if(world[i][j].bonr == -1)
	    world[i][j]=empty;
	  else{
	    world[world[i][j].bonr][world[i][j].bonc].bonr = -1;
	    world[i][j]=empty;
	  }
	}
      }
    }
}


/*Print to file time, kmut, kcat and sequence*/
int ToFile(char sign, int Time, double mut, double cat, char *seq, char who, int *anc)
{
  FILE *fp;
  int i;
  
  if (sign == '+')
  {
    fp = fopen("pmutatis.txt", "a");
    fprintf(fp, "%d  %f  %f  %s  %c  %d-", Time, mut, cat, seq, who, anc[0]);
    for(i=1; anc[i]!=0 && i<50;i++) fprintf(fp, "%d-",anc[i]);
    fprintf(fp, "\n");    
    fclose(fp);
  }
  else if (sign == '-')
  {
    fp = fopen("mmutatis.txt", "a");
    fprintf(fp, "%d  %f  %f  %s  %c  %d-", Time, mut, cat, seq, who, anc[0]);
    for(i=1; anc[i]!=0 && i<50;i++) fprintf(fp, "%d-",anc[i]);
    fprintf(fp, "\n");    
    fclose(fp);
  }
  else fprintf(stderr,"Hey, in function ToFile problems with + or -.\n");
  return 0;
}


/*******************************
 *          Diffusion          *
 *******************************/

/* You have to take care of what the empty cell is */
void RandWalk(TYPE2 **world, struct updorder* updorder_p,struct point** neighbor[])
{
  int i,irow,icol,irownei,icolnei;
  TYPE2 *icel,*icelnei;
  int length;
  TYPE2 empty;
  int nei;
  int bonr,bonc;
  
  empty = global_empty;

  length = nrow*ncol;

  for(i=0;i<length;i++) {
    irow=(updorder_p+i)->row;
    icol=(updorder_p+i)->col;
    
    icel=&world[irow][icol];

    /* if empty, then discard */
    if(icel->seq[0]=='N')
      continue;
    else 
      ; /* go on */

    /* chose a rondam neighbor */
    /* (the fractional part is truncated) */
    nei = 8.0*genrand_real2()+1; 
    irownei=neighbor[nei][irow][icol].row;
    icolnei=neighbor[nei][irow][icol].col;
    
    icelnei=&world[irownei][icolnei];
    
    /* if the neighbor is not empty, then discard */
    if(icelnei->seq[0]!='N')
      continue;
    else 
      ; /* go on */

    /* if simple morecule */
    if(icel->bonr==-1) {
      *icelnei=*icel;
      *icel=empty;
    }
    /*if complex molecule */
    else if(genrand_real1() < COMP_DIFF) {
      /* remember where the first part of molecule makes a bond */
      bonr = icel->bonr;
      bonc = icel->bonc;
      /* move the first part of molecule to icelnei */
      *icelnei=*icel;
      icelnei->bonr=irow; /* its other half of the molecule will
			     come to where it used to be */
      icelnei->bonc=icol;

      /* move the other half of the molecule to where the first
	 part of the molecule used to be */
      *icel=world[bonr][bonc];
      icel->bonr=irownei; /* the first part of the molecule is in
			     icelnei now */
      icel->bonc=icolnei;
      world[bonr][bonc]=empty;
    }
  }

}

/********************************
 *          Inputting           *
 ********************************/

int Args(int argc, char **argv)
{
  int i;
  int It;
  unsigned long ULt;
  double Dt;
  char	*Ct;
  //  char temp[BUFSIZE];
  /* parameters may be overridden here. */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i],"-Display") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It!=0)
	par_if_display=1;
      else if (It==0)
	par_if_display=0;
      else {
	fprintf(stderr,"Args: wrong value\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-Maxtime") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It>0)
	par_maxtime=It;
      else {
	fprintf(stderr,"Args: wrong value\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-Inittime") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It>0)
	par_inittime=It;
      else {
	fprintf(stderr,"Args: wrong value\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-Seed") == 0){
      i++; if (i == argc) return 1;
      ULt = strtoul(argv[i],(char**)NULL,10);
      if (ULt>0)
	par_ulseed=ULt;
      else {
	fprintf(stderr,"Args: wrong value\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-Boundary") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It>=0 && It<=3)
	boundary=It;
      else {
	fprintf(stderr,"Args: wrong value\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-File") == 0){
      i++; if(i==argc) return 1;
      Ct = argv[i];
      if ((strlen(Ct) > 0) && (strlen(Ct) <= BUFSIZE-1)) 
	strcpy(par_grace_outfile_name, Ct);
      else {
	fprintf(stderr,"Args: file name should be 1 to 200 in length\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-FileEnd") == 0){
      i++; if(i==argc) return 1;
      Ct = argv[i];
      if ((strlen(Ct) > 0) && (strlen(Ct) <= BUFSIZE-1)) 
	strcpy(endFileNameG, Ct);
      else {
	fprintf(stderr,"Args: file name should be 1 to 200 in length\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-FileGraph") == 0){
      i++; if(i==argc) return 1;
      Ct = argv[i];
      if ((strlen(Ct) > 0) && (strlen(Ct) <= BUFSIZE-1)) 
	strcpy(par_graph_outfile_name, Ct);
      else {
	fprintf(stderr,"Args: file name should be 1 to 200 in length\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-ReadFile") == 0){
      i++; if(i==argc) return 1;
      Ct = argv[i];
      if ((strlen(Ct) > 0) && (strlen(Ct) <= BUFSIZE-1))
	strcpy(par_input_file_name, Ct);
      else {
	fprintf(stderr,"Args: file name should be 1 to 200 in length\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-MovieDirName") == 0){
      i++; if(i==argc) return 1;
      Ct = argv[i];
      if ((strlen(Ct) > 0) && (strlen(Ct) <= BUFSIZE-1)){
	strcpy(par_movie_directory_name, Ct);
	par_if_movie = 1;
      }
      else {
	fprintf(stderr,"Args: file name should be 1 to 200 in length\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-Mut") == 0){
      i++; if (i == argc) return 1;
      Dt = atof(argv[i]);
      if (Dt>=0. && Dt<=1.)
	par_mutation=Dt;
      else {
	fprintf(stderr,"Args: wrong value for -Mut\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-MovieInt") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It>0)
	par_movie_interval=It;
      else {
	fprintf(stderr,"Args: wrong value for -MovieInt\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-BackupInt") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It>0)
	par_backup_interval=It;
      else {
	fprintf(stderr,"Args: wrong value for -BackupInt\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-SequenceInt") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It>0)
	par_out_sequence_interval=It;
      else {
	fprintf(stderr,"Args: wrong value for -BackupInt\n");
	return 1;
      }
      continue;
    }
    else if (strcmp(argv[i],"-GraceInt") == 0){
      i++; if (i == argc) return 1;
      It = atoi(argv[i]);
      if (It>0)
	par_xmgrace_interval=It;
      else {
	fprintf(stderr,"Args: wrong value for -BackupInt\n");
	return 1;
      }
      continue;
    }
    else {
      fprintf(stderr,"-Display -Maxtime -Inittime -Seed -Boundary (wrap 0;fixed 1) -File -FileEnd -FileGraph -ReadFile -MovieDirName -Mut -MovieInt -BackupInt -SequenceInt -GraceInt\n");
      exit(1);
    }
  }
  return (0);
}

int gotMouse(TYPE2 **world, int Time)
{
  char answer[BUFSIZE];
  char filename[BUFSIZE];
  int length;

  poseG = 0;
  while(1){
    printf("Enter: Contine (c), One step (o), Quit (q), Change (h), Save plane (s): ?");
    fgets(answer,BUFSIZE,stdin);
    switch(answer[0]){
    case 'c':
      return 0;
    case 'o':
      poseG = 1;
      return 0;
    case 'q':
      stopG = 1;
      return 0;
    case 'h':
      changeParam();
      break;
    case 's':
      printf("Type the name of file: ?");
      fgets(filename,BUFSIZE,stdin);
      length = strlen(filename);
      filename[length-1] = '\0';
      SavePlane(world,filename,Time);
      break;
    default:
      printf("Unknown command: %s\n",answer);
      break;
    }
  }

}

void changeParam(void)
{
  char answer[BUFSIZE];
  printf("Enter: Mutation (m) now local, you can't change it!, Display interval (d), Xmgrace interval (x), Movie on/off (p), Movie interval (v), # of mol in graph (n), # of back up interval (b), graph interval (g), par_k_threshold (k), par_m_threshold (i), maxtime (t), tree size <Mb> (s): ?");
  fgets(answer,BUFSIZE,stdin);
  switch(answer[0]){
/*  case 'm':
    printf("current value, par_mutation=%f. Type new rate:?",par_mutation);
    fgets(answer,BUFSIZE,stdin);
    par_mutation = atof(answer);
    if(par_mutation<0)
      par_mutation = 0.;
    break;
*/  case 'd':
    printf("current value, par_display_interval=%d. Type new interval:?",par_display_interval);
    fgets(answer,BUFSIZE,stdin);
    par_display_interval = atoi(answer);
    if(par_display_interval<=0)
      par_display_interval = 1;
    break;
  case 'x':
    printf("current value, par_xmgrace_interval=%d. Type new interval:?",par_xmgrace_interval);
    fgets(answer,BUFSIZE,stdin);
    par_xmgrace_interval = atoi(answer);
    if(par_xmgrace_interval<=0)
      par_xmgrace_interval = 1;
    break;
  case 'p':
    printf("current value, par_if_movie=%d (0:off / 1: on). Type new value:?",par_if_movie);
    fgets(answer,BUFSIZE,stdin);
    par_movie_interval = atoi(answer);
    if(par_if_movie <=0)
      par_if_movie = 0;
    break;
  case 'v':
    printf("current value, par_movie_interval=%d. Type new interval:?",par_movie_interval);
    fgets(answer,BUFSIZE,stdin);
    par_movie_interval = atoi(answer);
    if(par_movie_interval <=0)
      par_movie_interval = 1;
    break;
  case 'b':
    printf("current value, par_backup_interval=%d. Type new interval:?",par_backup_interval);
    fgets(answer,BUFSIZE,stdin);
    par_backup_interval = atoi(answer);
    if(par_backup_interval<=0)
      par_backup_interval = 10000;
    break;
  case 'i':
    printf("current value, par_m_threshold=%d. Type new threshold:?",par_m_threshold);
    fgets(answer,BUFSIZE,stdin);
    par_m_threshold = atoi(answer);
    if(par_m_threshold <= 0)
      par_m_threshold = 1;
    break;
  case 't':
    printf("current value, par_maxtime=%d. Type new value:?",par_maxtime);
    fgets(answer,BUFSIZE,stdin);
    par_maxtime = atoi(answer);
    if(par_maxtime <= 0)
      par_maxtime = 0;
    break;
  case 's':
    printf("current value, par_cash_size=%d. Type new size:?",par_cash_size);
    fgets(answer,BUFSIZE,stdin);
    par_cash_size = atoi(answer);
    if(par_cash_size < 0)
      par_cash_size = 0;
    nNodeMax = par_cash_size*1000*1000/sizeof(TYPE2);
    break;
  default:
    printf("Unknown parameter: %s\n",answer);
    break;
  }
}

void SigCatch(int sig)
{
  signalStopG=1;

  /* Let reaction to this signal be reset */
  if(SIG_ERR == signal(SIGUSR1,SigCatch)){
    fprintf(stderr,"SigCatch: failed to reset signalhandler.\n");
  }
}

/****************************************
 *              Finishing               *
 ****************************************/
int RemoveEndFile(void)
{
  char command[200];
  FILE *removeCommand;

  if(strlen(endFileNameG)==0)
    return 0;

  sprintf(command,"/bin/rm -f %s",endFileNameG);
  removeCommand=popen(command,"r");
  if(pclose(removeCommand)==-1)
    fprintf(stderr,"RemoveEndFile: error in removing endfile\n");

  return 1;
}

void Finish(TYPE2 **world,struct point **neighbor[],struct updorder *updorder_p)
{
  int i;
  /* See simu.c if possible */
  RemoveEndFile();
  PlaneFree2(world);
  for(i=0;i<9;i++)
    NeighFree(neighbor[i]);
  free(updorder_p);
}


void ErrorFinish(TYPE2**world,int Time)
{
  char fname[BUFSIZE];

  /* Save the plane, back-up */
  sprintf(fname,"%s_t=%d",par_error_backup_file_name,Time);
  SavePlane(world,fname,Time);

  /* See simu.c if possible */
  RemoveEndFile();

  /* output the time of finish */
  fprintf(stderr,"Error Finished at:\n%s\n",OutputTime());

  exit(-1);
}

void SignalStop(TYPE2**world,int Time)
{
  char fname[BUFSIZE];
  char command[512]; 

  /* Save the plane, back-up */
  sprintf(command,"%s %s","mkdir -p",par_backup_directory_name);
  if(system(command)==-1){
    fprintf(stderr,"SignalFinish: Failed to mkdir %s. Save file here.\n",par_backup_directory_name);
    sprintf(fname,"%s_t%d",par_backup_file_name,Time);
  }else{
    sprintf(fname,"%s/%s_t%d",par_backup_directory_name,par_backup_file_name,Time);
  }
  SavePlane(world,fname,Time);


  /* See simu.c if possible */
  RemoveEndFile();
}

/***************************
 *       Outputting        *
 ***************************/
/* Return the current time */
char *OutputTime(void)
{
  time_t tp;
  struct tm *tpp;
  char *s;
  time(&tp);
  tpp=localtime(&tp);
  s=asctime(tpp);
  return s;
}

/* This function will call all the other outputting
   functions. This also checks if the population is extinct; if
   so, it will set stopG to be 1. */
void OutputAll(TYPE2 **world,int Time)
{
  if((Time % par_backup_interval) == 0 && Time!=0)
    OutputBackup(world,Time);
}

void OutputBackup(TYPE2 **world,int Time)
{
  char fname[BUFSIZE];
  char command[512];
    
  sprintf(command,"%s %s","mkdir -p",par_backup_directory_name);
  if(system(command)==-1){
    fprintf(stderr,"OutputBackup: Failed to mkdir %s. Save here\n",par_backup_directory_name);
    sprintf(fname,"%s_t%d",par_backup_file_name,Time);
  }
  else
    sprintf(fname,"%s/%s_t%d",par_backup_directory_name,par_backup_file_name,Time);
  
  SavePlane(world,fname,Time);
}

int SavePlane(TYPE2 **world,char *fname,int Time)
{
  int nr=nrow,nc=ncol;
  int i,j,control=0;
  FILE *fout;

  if((fout = fopen(fname,"a"))==NULL){
    fprintf(stderr,"SavePlane: cannot open %s\n",fname);
    return -1;
  }

  /* The size of field */
  fprintf(fout,"# %d X %d\n",nr,nc);

  for(i=1;i<=nr;i++)
    for(j=1;j<=nc;j++){
      if(world[i][j].seq[0]!='N'){
        control++;
	fprintf(fout,"%s\n%s\n",world[i][j].seq,world[i][j].str);
        /*fprintf(fout,"%f\n",world[i][j].kmut);*//*If you put kmut in backup, also include another 'N' in case of empty spot*/
	if(world[i][j].anc[49]==0) world[i][j].anc[(Time-par_inittime)/par_backup_interval]= i*nr+j;

      }
      else
	fputs("N\nN\n",fout);
        /*fputs("N\n",fout);*/
	
    }
  if(control==0) signalStopG=1;
  fclose(fout);
  return 0;
}

