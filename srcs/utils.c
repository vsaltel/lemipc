#include "lemipc.h"
#include <sys/wait.h>

void	init_lem(t_lem *lem)
{
	int	i;

	lem->shm = NULL;
	lem->main_player = 0;
	lem->nb_player = 0;
	lem->player_id = 0;
	lem->team = 2;
	lem->alive = 1;
	lem->msgqid = -1;
	i = -1;
	while (++i < MAX_PLAYER - 1)
		lem->pids[i] = 0;
}

void	exit_free(t_lem *lem)
{
	free_msgq(lem);
	sem_destroy(lem->semid);
	free_shm(lem->shm);
	exit(0);
}

void	catch_sigint(int signal)
{
	if (lem.pid)
	{
		ft_dprintf(2, "lemipc: catch sig %d\n", signal);
		exit_free(&lem);
	}
	exit(0);
}
