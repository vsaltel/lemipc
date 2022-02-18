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

static void	wait_child_die(t_lem *lem)
{
    int status;
	int	nb;
	int	ret;

	nb = 0;
    while (!lem->pids[nb] || nb > (MAX_PLAYER - 1))
    {
		ret = waitpid((pid_t)lem->pids[nb], &status, WNOHANG);
		if (ret == -1)
			return;
		ft_printf("%d die\n", lem->pids[nb]);
		nb++;
    }
}

void	free_shm(t_shm *shm)
{
	if (shm)
		munmap(shm, sizeof(t_lem));
}

void	exit_free(t_lem *lem)
{
	kill_process(lem);
	wait_child_die(lem);
	free_msgq(lem);
	sem_destroy(lem->semid);
	free_shm(lem->shm);
	restore_shell_input(lem);
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

void	*create_shared_memory(size_t size)
{
	int protection;
	int visibility;

	protection = PROT_READ | PROT_WRITE; 
 	visibility = MAP_SHARED | MAP_ANONYMOUS;
	return (mmap(NULL, size, protection, visibility, -1, 0));
}
