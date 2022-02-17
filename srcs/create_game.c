#include "lemipc.h"

static int	get_nb_player(t_lem *lem)
{
	char	*input;

	lem->main_player = 1;
	ft_printf("Number of players (multiple of 2): ");
	if (get_next_line(0, &input) < 0)
		return (1);
	lem->nb_player = ft_atoi(input);
	free(input);
	if (lem->nb_player <= 0)
		return (ft_mret("lemipc: No player\n", 2));
	if (lem->nb_player % 2)
		return (ft_mret("lemipc: Not a multiple of 2\n", 3));
	if (lem->nb_player > MAX_PLAYER)
		return (ft_mret("lemipc: to many players\n", 4));
	return (0);
}

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
	lem->shm->player_round = lem->nb_player;
	lem->shm->game_state = 1;
}

void	set_initial_position(t_lem *lem)
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

int	load_game(t_lem	*lem)
{
	while (++lem->player_id < lem->nb_player)
	{
		set_initial_position(lem);
		lem->pid = fork();
		if (lem->pid < 0)
			return (ft_mret("lemipc: fork() fail\n", 11));
		if (lem->pid) //parent
			lem->pids[lem->player_id - 1] = lem->pid;
		else
			break;
		lem->team = (lem->team == 1) ? (lem->team = 2) : (lem->team = 1);
	}
	if (lem->pid)
	{
		set_initial_position(lem);
		init_shell_input(lem);
		main_player(lem);
		exit_free(lem);
	}
	else
		other_player(lem);
	return (0);
}

int	create_game(t_lem *lem)
{
	int	ret;	

	ret = 0;
	if ((ret = get_nb_player(lem)))
		return (ret);
	lem->shm = create_shared_memory(sizeof(t_shm));
	if (lem->shm == MAP_FAILED)
		return (5);
	init_shm(lem);
	lem->semid = sem_alloc();
	sem_init(lem->semid);
	ret = load_game(lem);
	return (ret);
}
