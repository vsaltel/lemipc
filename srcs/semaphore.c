# include "lemipc.h"
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/sem.h>

int		sem_alloc(t_lem *lem)
{
	int key;

	if ((key = ftok("/tmp", 65)) == -1)
	{
		perror("ftok");
		exit(1);
	}
	lem->semid = semget(key, 1, 0);
	if (errno == ENOENT)
		lem->semid = semget(key, 1, 0666 | IPC_CREAT);
	if (lem->semid == -1)
	{
		perror("semget");
		exit(2);
	}
	return (lem->semid);
}

int		sem_destroy(t_lem *lem)
{
	int ret;

	ret = semctl(lem->semid, 1, IPC_RMID, 0);
	if (ret == -1)
		perror("semctl");
	return (ret);
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
