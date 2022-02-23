#include "lemipc.h"

static void	init_shm(t_lem *lem)
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

static void	set_team(t_lem *lem)
{
	int x;
	int y;
	int	t;
	int	pt;
	int	nb;
	int	nb_bef;

	nb_bef = MAX_PLAYER;
	pt = 1;
	t = 0;
	while (++t <= lem->nb_team)
	{
		nb = 0;
		y = -1;
		while (++y < MAP_SIZE)
		{
			x = -1;
			while (++x < MAP_SIZE)
				if (lem->shm->area[y][x] == t)
				 nb++;
		}
		if (nb < nb_bef)
		{
			nb_bef = nb;
			pt = t;
		}
	}
	lem->team = pt;
	ft_printf("Team : %d\n", pt);
}

static void	set_initial_position(t_lem *lem)
{
	int	r;

	while (1)
	{
		srand(time(NULL));
		r = rand();
		lem->y = (r >> (sizeof(int) * 8) / 2) % MAP_SIZE;
		lem->x = (r << (sizeof(int) * 8) / 2) % MAP_SIZE;
		if (lem->x < 0)
			lem->x = -lem->x;
		if (!lem->shm->area[lem->y][lem->x])
		{
			sem_wait(lem->semid);
			lem->shm->area[lem->y][lem->x] = lem->team;
			sem_post(lem->semid);
			break;
		}
	}
}

static void	wait_players(t_lem *lem)
{
	int tmp;
	int nb;

	if (!WAIT_PLAYER)
		return;
	nb = 0;
	while (1)
	{
		if ((tmp = check_nb_player(lem)) >= WAIT_PLAYER)
			break;
		if (tmp != nb)
		{
			nb = tmp;
			ft_printf("Waiting players : %d/%d\n", nb, WAIT_PLAYER);
		}
		usleep(100);
	}
}

static void	load_game(t_lem	*lem)
{
	signal(SIGINT, &catch_sigint);
	set_team(lem);
	set_initial_position(lem);
	wait_players(lem);
	player(lem);
	if (check_if_empty(lem))
		exit_free(lem);
}

int	create_game(t_lem *lem)
{
	create_shared_memory(lem);
	if (lem->creator)
		init_shm(lem);
	sem_alloc(lem);
	sem_init(lem->semid);
	join_msgq(lem);
	load_game(lem);
	return (0);
}
