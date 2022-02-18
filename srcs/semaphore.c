# include "lemipc.h"
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/sem.h>

int		sem_alloc(void)
{
	int	sem_flags;

	sem_flags = IPC_CREAT;
	return (semget(IPC_PRIVATE, 1, sem_flags));
}

int		sem_destroy(int semid)
{
	//union semun	ignored_argument;

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
