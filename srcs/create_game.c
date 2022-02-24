#include "lemipc.h"

static void	set_team(t_lem *lem)
{
	int team;
	int	count;
	int	count_tmp;
	int	i;

	team = 1;
	count = check_nb_player_team(lem, 1);
	i = 1;
	while (++i <= lem->nb_team)
	{
		count_tmp = check_nb_player_team(lem, i);
		if (count_tmp < count)
		{
			team = i;
			count = count_tmp;
		}
	}
	lem->team = team;
	ft_printf("Team : %d\n", lem->team);
}

static void	set_initial_position(t_lem *lem)
{
	int	r;

	r = 0;
	while (1)
	{
		srand((time(NULL) * lem->pid) + r);
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
		tmp = check_nb_player(lem);
		if (tmp >= WAIT_PLAYER)
			break;
		if (tmp != nb)
		{
			nb = tmp;
			ft_printf("Waiting players : %d/%d\n", nb, WAIT_PLAYER);
		}
		usleep(30);
	}
}

static void	load_game(t_lem	*lem)
{
	signal(SIGINT, &catch_sigint);
	if (lem->c)
		init_shell_input(lem);
	set_team(lem);
	set_initial_position(lem);
	wait_players(lem);
	usleep(100);
	player(lem);
	if (lem->c)
		restore_shell_input(lem);
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
