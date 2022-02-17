#include "lemipc.h"

void	display_map(t_lem *lem)
{
	int		i;
	int		y;
	char	c;

	y = -1;
	while (++y < MAP_SIZE)
	{
		i = -1;
		while (++i < MAP_SIZE)
		{
			c = '0' + lem->shm->area[y][i];
			if (y == lem->y && i == lem->i)
				ft_printf("\033[0;31m%c\033[0m", c);	
			else
				write(1, &c, 1);
			write(1, " ", 1);
		}
		write(1, "\n", 1);
	}
}
