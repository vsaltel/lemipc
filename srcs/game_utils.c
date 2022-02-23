#include "lemipc.h"

int		check_nearly_ennemi(t_lem *lem, int *dy, int *dx)
{
	int	x;
	int	y;

	y = lem->y - 2;
	while (y < 0)
		y++;
	while (y <= lem->y + 1)
	{
		x = lem->x - 2;
		while (x < 0)
			x++;
		while (x <= lem->x + 1)
		{
			if (x < MAP_SIZE && y < MAP_SIZE &&
				lem->shm->area[y][x] && lem->shm->area[y][x] != lem->team)
			{
				*dy = y;
				*dx = x;
				return (1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

int		check_if_empty(t_lem *lem)
{
	int	x;
	int	y;

	y = -1;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
		{
			if (lem->shm->area[y][x])
				return (0);
		}
	}
	return (1);
}

int		check_if_last_team(t_lem *lem)
{
	int	x;
	int	y;

	y = -1;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			if (lem->shm->area[y][x] != 0 && lem->shm->area[y][x] != lem->team)
				return (0);
	}
	return (1);
}

int		check_nb_player_team(t_lem *lem)
{
	int	x;
	int	y;
	int	nb;

	nb = 0;
	y = -1;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			if (lem->shm->area[y][x] == lem->team)
				nb++;
	}
	return (nb);
}

int		check_nb_player(t_lem *lem)
{
	int	x;
	int	y;
	int	nb;

	nb = 0;
	y = -1;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			if (lem->shm->area[y][x])
				nb++;
	}
	return (nb);
}

int		check_if_encircled(t_lem *lem)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = (((lem->y - 1) < 0) ? 0 : lem->y - 1);
	while (y <= lem->y + 1)
	{
		x = (((lem->x - 1) < 0) ? 0 : lem->x - 1);
		while (x <= lem->x + 1)
		{
			if (x < MAP_SIZE && y < MAP_SIZE &&
				lem->shm->area[y][x] && lem->shm->area[y][x] != lem->team)
				count++;
			x++;
		}
		y++;
	}
	if (count >= 2)
		return (1);
	return (0);
}
