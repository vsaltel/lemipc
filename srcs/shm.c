#include "lemipc.h"
#include <sys/ipc.h>
#include <sys/shm.h>

void	*create_shared_memory(t_lem *lem)
{
    int key;
  
    if ((key = ftok("/tmp", 65)) == -1)
	{
		perror("ftok");
		exit(1);
	}
	lem->shmid = shmget(key, sizeof(t_shm), 0666);
	if (errno == ENOENT)
	{
		ft_printf("create shm\n");
		lem->shmid = shmget(key, sizeof(t_shm), 0666 | IPC_CREAT);
		lem->creator = 1;
	}
	if (lem->shmid == -1)
	{
		perror("shmget");
		exit(1);
	}
	lem->shm = shmat(lem->shmid, NULL, 0);
	return (lem->shm);
}

int	free_shm(t_lem *lem)
{
	int	ret;

	ret = shmctl(lem->shmid, IPC_RMID, 0);
	if (ret == -1)
		perror("shmctl");
	return (ret);
}
