#ifndef SEMINC
#define SEMINC
#include <sys/sem.h>




#define P -1
#define V 1


void P_op(int semid, int numid);
void V_op(int semid, int numid);
void semOperation(int semid, int op, int id);


#endif