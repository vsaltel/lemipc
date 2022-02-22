#include "lemipc.h"

int		check_if_empty(t_lem *lem)
{
	int	i;
	int	y;

	y = -1;
	while (++y < MAP_SIZE)
	{
		i = -1;
		while (++i < MAP_SIZE)
		{
			if (lem->shm->area[y][i])
				return (0);
		}
	}
	return (1);
}

int		check_if_last_team(t_lem *lem)
{
	int	i;
	int	y;

	y = -1;
	while (++y < MAP_SIZE)
	{
		i = -1;
		while (++i < MAP_SIZE)
			if (lem->shm->area[y][i] != 0 && lem->shm->area[y][i] != lem->team)
				return (0);
	}
	return (1);
}

int		check_nb_player(t_lem *lem)
{
	int	i;
	int	y;
	int	nb;

	nb = 0;
	y = -1;
	while (++y < MAP_SIZE)
	{
		i = -1;
		while (++i < MAP_SIZE)
			if (lem->shm->area[y][i])
				nb++;
	}
	return (nb);
}

int		check_if_encircled(t_lem *lem)
{
	int	count;
	int	y;
	int	i;

	count = 0;
	y = ((lem->y - 1) >= 0) ? (lem->y - 1) : lem->y;
	while (y <= lem->y + 1)
	{
		i = ((lem->i - 1) >= 0) ? (lem->i - 1) : lem->i;
		while (i <= lem->i + 1)
		{
			if (i < MAP_SIZE && y < MAP_SIZE &&
				lem->shm->area[y][i] && lem->shm->area[y][i] != lem->team)
				count++;
			i++;
		}
		y++;
	}
	if (count >= 2)
		return (1);
	return (0);
}
