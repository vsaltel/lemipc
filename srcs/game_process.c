#include "lemipc.h"

static void	basic_move(t_lem *lem)
{
	static int n;

	srand((time(NULL) / lem->pid) + n);
	n = rand() % 3;
	move_player(lem, n);
	/*
	if (lem->pid % 2)
		n++;
	else
		n++;
	if (n > 3)
		n = 0;
	if (n < 0)
		n = 3;
	*/
}

static void	advanced_move(t_lem *lem)
{
	t_msgq	msgq;
	int		x;
	int		y;

	if (receive_message(lem, &msgq) && msgq.mes.pid != lem->pid && msgq.mesg_type == lem->team)
		move_to_target(lem, msgq.mes.y, msgq.mes.x);
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
	if (lem->c)
		control_player(lem);
	else if (lem->team == 1)
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
			free_space(lem);
			ft_printf("Your team win\n");
			return ;
		}
		if (lem->alive && check_equality(lem))
		{
			free_space(lem);
			ft_printf("Equality\n");
			return ;
		}
		if (lem->alive && check_if_encircled(lem))
		{
			free_space(lem);
			lem->alive = 0;
			ft_printf("You are dead\n");
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
		sleep(SLEEP_VALUE);
	}
}
