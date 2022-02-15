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
}

int	load_game(t_lem	*lem)
{
	int		n;

	n = 0;
	while (++n < lem->nb_player)
	{
		if (n == (lem->nb_player / 2) + 1)
			lem->team = 2;
		lem->pid = fork();
		if (lem-> pid < 0)
			return (ft_mret("lemipc: fork() fail\n", 6));
		if (!lem->pid) //child
			break;
	}
	if (lem->pid)
	{
		main_player(lem);
		free_shm(lem->shm);
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
	ret = load_game(lem);
	return (ret);
}
