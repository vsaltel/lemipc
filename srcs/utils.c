#include "lemipc.h"
#include <sys/wait.h>

void	init_lem(t_lem *lem)
{
	lem->shm = NULL;
	lem->main_player = 0;
	lem->nb_player = 0;
	lem->creator = 0;
	lem->nb_team = 2;
	lem->team = 2;
	lem->alive = 1;
	lem->msgqid = -1;
}

void	exit_free(t_lem *lem)
{
	ft_printf("free all\n");
	free_msgq(lem);
	sem_destroy(lem);
	free_shm(lem);
	exit(0);
}

void	catch_sigint(int signal)
{
	ft_dprintf(2, "lemipc: catch sig %d\n", signal);
	sem_wait(lem.semid);
	lem.shm->area[lem.y][lem.x] = 0;
	sem_post(lem.semid);
	if (check_if_empty(&lem))
		exit_free(&lem);
	exit(0);
}
