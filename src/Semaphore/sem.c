#include "sem.h"
 
void P_op(int semid, int numid)
{
    semOperation(semid, P, numid);
}

void V_op(int semid, int numid)
{
    semOperation(semid, V, numid);
}

void semOperation(int semid, int op, int id)
{
    struct sembuf operation;

    operation.sem_num = id;
    operation.sem_op = op;
    operation.sem_flg = SEM_UNDO;

    semop(semid, &operation, 1);
}
