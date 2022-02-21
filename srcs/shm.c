#include "lemipc.h"

void	*create_shared_memory(t_lem *lem)
{
    int key;
	int	shmid;
  
    key = ftok(SHM_NAME, 65);
	if ((lem->shmid = shmget(key, sizeof(t_shm), 0644)) == ENOENT)
		lem->shmid = shmget(key, sizeof(t_shm), 0644 | IPC_CREAT);
	if (lem->shmid == -1)
	{
		perror("shmget");
		exit(1);
	}
	lem->shm = shmat(shmid, NULL, 0);
	return (lem->shm);
}

void	free_shm(t_shm *shm)
{
	return (semctl(lem->shmid, IPC_RMID, 0));
}
