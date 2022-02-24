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
		lem->shmid = shmget(key, sizeof(t_shm), 0666 | IPC_CREAT);
		if (lem->shmid != -1 && lem->v)
			ft_printf("shm created\n");
		lem->creator = 1;
	}
	else if (lem->shmid != -1 && lem->v)
		ft_printf("shm joined\n");
	if (lem->shmid == -1)
	{
		perror("shmget");
		exit(1);
	}
	lem->shm = shmat(lem->shmid, NULL, 0);
	return (lem->shm);
}

int		free_shm(t_lem *lem)
{
	return (shmctl(lem->shmid, IPC_RMID, 0));
}

void	init_shm(t_lem *lem)
{
	int		x;
	int		y;

	y = -1;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			lem->shm->area[y][x] = 0;
	}
}
