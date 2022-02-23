#include "lemipc.h"

static void	basic_move(t_lem *lem)
{
	static int n;

	if (lem->team == 1)
	{
	if (move_player(lem, n))
		n++;
	if (n > 3)
		n = 0;
	}
}

static void	advanced_move(t_lem *lem)
{
	t_msgq	msgq;
	int		x;
	int		y;

	if (receive_message(lem, &msgq)/* && msgq.mesg_type == lem->team*/)
	{
		move_to_target(lem, msgq.mes.y, msgq.mes.x);
	}
	else if (check_nearly_ennemi(lem, &y, &x))
	{
		send_target_msg(lem, y, x);
		move_to_target(lem, y, x);
	}
	else
		basic_move(lem);
}

void	player_process(t_lem *lem)
{
	if (lem->team == 1)
		advanced_move(lem);
	else
		basic_move(lem);
}

void	player(t_lem *lem)
{
	while (1)
	{
		if (lem->alive && check_if_last_team(lem))
		{
			sem_wait(lem->semid);
			lem->shm->area[lem->y][lem->x] = 0;
			sem_post(lem->semid);
			ft_printf("Your team win\n");
			return ;
		}
		if (lem->alive && check_if_encircled(lem))
		{
			sem_wait(lem->semid);
			lem->shm->area[lem->y][lem->x] = 0;
			sem_post(lem->semid);
			lem->alive = 0;
			if (!lem->creator)
				return ;
		}
		if (lem->alive)
			player_process(lem);
		if (lem->creator)
		{
			display_map(lem);
			if (!lem->alive && check_if_empty(lem))
				return ;
		}
		sleep(1);
	}
}
