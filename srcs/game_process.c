#include "lemipc.h"

void	player_process(t_lem *lem)
{
	if (move_player(lem, RIGHT))
		if (move_player(lem, UP))
			move_player(lem, LEFT);
}

void	player(t_lem *lem)
{
	//t_msgq	msgq;

	while (1)
	{
		if (lem->alive && check_if_last_team(lem))
		{
			ft_printf("Your team win\n");
			return ;
		}
		if (lem->alive && check_if_encircled(lem))
		{
			ft_printf("You are dead\n");
			lem->alive = 0;
		}
		if (!lem->alive && !lem->creator)
			return ;
		if (lem->alive)
			player_process(lem);
		if (lem->creator)
		{
			display_map(lem);
			if (!lem->alive && check_if_last_team(lem))
				return ;
		}
		sleep(1);
	}
}
