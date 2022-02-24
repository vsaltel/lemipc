#include "lemipc.h"
#include <sys/wait.h>

void	init_lem(t_lem *lem)
{
	lem->shm = NULL;
	lem->creator = 0;
	if ((lem->nb_team = NB_TEAM) < 2)
		lem->nb_team = 2;
	lem->team = 2;
	lem->alive = 1;
	lem->msgqid = -1;
	lem->sem_taken = 0;
	lem->v = 0;
	lem->c = 0;
	lem->pid = getpid();
}

void	free_space(t_lem *lem)
{
	sem_wait(lem->semid);
	lem->shm->area[lem->y][lem->x] = 0;
	sem_post(lem->semid);
}

void	exit_free(t_lem *lem)
{
	free_msgq(lem);
	sem_destroy(lem);
	free_shm(lem);
	exit(0);
}

void	catch_sigint(int signal)
{
	ft_dprintf(2, "lemipc: catch sig %d\n", signal);
	if (lem.sem_taken)
		sem_post(lem.semid);
	free_space(&lem);
	if (lem.c)
		restore_shell_input(&lem);
	if (check_if_empty(&lem))
		exit_free(&lem);
	exit(0);
}
