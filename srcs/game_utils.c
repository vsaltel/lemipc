#include "lemipc.h"

int		check_nearly_ennemi(t_lem *lem, int *dy, int *dx)
{
	int	x;
	int	y;
	int	value;
	int	tvalue;

	value = MAP_SIZE + MAP_SIZE;
	y = -1;
	sem_wait(lem->semid);
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			if (lem->shm->area[y][x] && lem->shm->area[y][x] != lem->team)
			{
				tvalue = ((lem->x > x) ? lem->x - x : x - lem->x) + ((lem->x > y) ? lem->y - y : y - lem->y);
				if (tvalue < value)
				{
					*dx = x;
					*dy = y;
					value = tvalue;
				}
			}
	}
	sem_post(lem->semid);
	return (1);
}

int		check_if_empty(t_lem *lem)
{
	int	x;
	int	y;

	y = -1;
	sem_wait(lem->semid);
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
		{
			if (lem->shm->area[y][x])
			{
				sem_post(lem->semid);
				return (0);
			}
		}
	}
	sem_post(lem->semid);
	return (1);
}

int		check_if_last_team(t_lem *lem)
{
	int	x;
	int	y;

	y = -1;
	sem_wait(lem->semid);
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			if (lem->shm->area[y][x] != 0 && lem->shm->area[y][x] != lem->team)
			{
				sem_post(lem->semid);
				return (0);
			}
	}
	sem_post(lem->semid);
	return (1);
}

int		check_nb_player_team(t_lem *lem, int team)
{
	int	x;
	int	y;
	int	nb;

	nb = 0;
	y = -1;
	sem_wait(lem->semid);
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			if (lem->shm->area[y][x] == team)
				nb++;
	}
	sem_post(lem->semid);
	return (nb);
}

int		check_nb_player(t_lem *lem)
{
	int	x;
	int	y;
	int	nb;

	nb = 0;
	y = -1;
	sem_wait(lem->semid);
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			if (lem->shm->area[y][x])
				nb++;
	}
	sem_post(lem->semid);
	return (nb);
}

int		check_if_encircled(t_lem *lem)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = (((lem->y - 1) < 0) ? 0 : lem->y - 1);
	sem_wait(lem->semid);
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
	sem_post(lem->semid);
	if (count >= 2)
		return (1);
	return (0);
}

int		check_equality(t_lem *lem)
{
	int	i;

	i = 0;
	while (++i <= lem->nb_team)
		if (check_nb_player_team(lem, i) > 1)
			return (0);
	return (1);
}
