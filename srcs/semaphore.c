# include "lemipc.h"
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/sem.h>

int		sem_alloc(t_lem *lem)
{
	if ((lem->semid = semget(IPC_PRIVATE, 1, 0)) == ENOENT)
		lem->semid = semget(IPC_PRIVATE, 1, IPC_CREAT);
	if (lem->semid == -1)
	{
		perror("semget");
		exit(2);
	}
	return (lem->semid);
}

int		sem_destroy(int semid)
{
	return (semctl(semid, 1, IPC_RMID, 0));
}

int		sem_init(int semid)
{
	union semun_u	argument;
	unsigned short	values[1];

	values[0] = 1;
	argument.array = values;
	return (semctl(semid, 0, SETALL, argument));
}

int		sem_wait(int semid)
{
	struct sembuf operations[1];

	operations[0].sem_num = 0;
	operations[0].sem_op = -1;
	operations[0].sem_flg = SEM_UNDO;
	return (semop(semid, operations, 1));
}

int		sem_post(int semid)
{
	struct sembuf operations[1];

	operations[0].sem_num = 0;
	operations[0].sem_op = 1;
	operations[0].sem_flg = SEM_UNDO;
	return (semop(semid, operations, 1));
}
