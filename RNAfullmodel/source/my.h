#ifndef MY
#define MY

/* Initilization */
void Initial(TYPE2 **);
void testworld(TYPE2 **);
void ReadWorldPart(TYPE2 **);

/* Molecular dynamics */
void MoleculeAction(TYPE2 **,struct updorder*,struct point**[],int);
void MoleculeDecay(TYPE2 **world);

/*Change mutation rate*/
double ChangeMut(double mut);

/*Output to pmutatis.txt*/
int ToFile(char sign, int Time, double mut, double cat, char *seq, char who, int *anc);

/* Input */
int Args(int,char **);
int gotMouse(TYPE2 **, int Time);
void changeParam(void);
void SigCatch(int); /* signal handler */

/* Finishing */
void Finish(TYPE2 **world,struct point **neighbor[],struct updorder *updorder_p);
int RemoveEndFile(void);
void ErrorFinish(TYPE2**,int);
void SignalStop(TYPE2**,int);

/* Output etc. */
char *OutputTime(void);
void OutputAll(TYPE2**,int);
void OutputBackup(TYPE2 **,int);
int SavePlane(TYPE2 **,char *, int Time);

/* Diffusion */
void RandWalk(TYPE2 **,struct updorder *,struct point **[]);

#endif //My
