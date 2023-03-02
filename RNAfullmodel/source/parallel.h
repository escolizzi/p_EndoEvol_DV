#include <pthread.h>
#include "cash2.h"


void parallelInit(TYPE2 **world,char *target);
void parallelClean(void);
void parallelMoleculeAction(TYPE2 **world,struct updorder* updorder_p,struct point** neighbor[],double mutation);

