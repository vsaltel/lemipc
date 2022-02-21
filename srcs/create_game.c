#include "lemipc.h"

static void	init_shm(t_lem *lem)
{
	int		i;
	int		y;

	y = -1;
	while (++y < MAP_SIZE)
	{
		i = -1;
		while (++i < MAP_SIZE)
			lem->shm->area[y][i] = 0;
	}
	lem->shm->game_state = 1;
}

static void	set_initial_position(t_lem *lem)
{
	int	i;
	int	r;

	i = 1;
	while (i)
	{
		srand(time(NULL));
		r = rand();
		lem->y = (r >> (sizeof(int) * 8) / 2) % MAP_SIZE;
		lem->i = (r << (sizeof(int) * 8) / 2) % MAP_SIZE;
		if (lem->i < 0)
			lem->i = -lem->i;
		sem_wait(lem->semid);
		if (!lem->shm->area[lem->y][lem->i])
		{
			lem->shm->area[lem->y][lem->i] = lem->team;
			i = 0;
		}
		sem_post(lem->semid);
	}
}

static void	load_game(t_lem	*lem)
{
	signal(SIGINT, &catch_sigint);
	set_initial_position(lem);
	player(lem);
	if (check_if_last(lem))
		exit_free(lem);
	return (0);
}

int	create_game(t_lem *lem)
{
	create_shared_memory(lem);
	init_shm(lem);
	sem_alloc(lem);
	sem_init(lem->semid);
	join_msgq(lem);
	ft_dprintf(2, "msgid %d\n", lem->msgqid);
	load_game(lem);
	return (0);
}
