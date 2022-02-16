#include "lemipc.h"

void	init_lem(t_lem *lem)
{
	int	i;

	lem->shm = NULL;
	lem->main_player = 0;
	lem->nb_player = 0;
	lem->player_id = 0;
	lem->team = 1;
	i = -1;
	while (++i < MAX_PLAYER - 1)
		lem->pids[i] = 0;
}

static void	kill_process(t_lem *lem)
{
	int	i;

	i = -1;
	while (++i < MAX_PLAYER)
	{
		if (lem->pids[i])
			kill(lem->pids[i], SIGINT);
	}
}

void	catch_sigint(int signal)
{
	if (lem.pid)
	{
		ft_dprintf(2, "lemipc: catch sig %d\n", signal);
		kill_process(&lem);
		sem_destroy(lem.semid);
	}
	exit(0);
}

void	free_shm(t_shm *shm)
{
	if (shm)
		munmap(shm, sizeof(t_lem));
}

void	*create_shared_memory(size_t size)
{
	int protection;
	int visibility;

	protection = PROT_READ | PROT_WRITE; 
 	visibility = MAP_SHARED | MAP_ANONYMOUS;
	return (mmap(NULL, size, protection, visibility, -1, 0));
}
