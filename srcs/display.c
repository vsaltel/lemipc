#include "lemipc.h"

void	display_map(t_lem *lem)
{
	int		x;
	int		y;
	char	c;

	y = -1;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
		{
			c = '0' + lem->shm->area[y][x];
			if (lem->alive && y == lem->y && x == lem->x)
				ft_printf("\033[0;31m%c\033[0m", c);	
			else
				write(1, &c, 1);
			write(1, " ", 1);
		}
		write(1, "\n", 1);
	}
	write(1, "\n", 1);
}
