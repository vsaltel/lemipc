#include "lemipc.h"

static void	basic_move(t_lem *lem)
{
	static int n;

	if (!n)
		n = (lem->pid % 3) + 1;
	move_player(lem, n - 1);
	if (++n == 5)
		n = 1;
}

static void	advanced_move(t_lem *lem)
{
	t_msgq	msgq;
	int		x;
	int		y;

	if (receive_message(lem, &msgq) && msgq.mes.pid != lem->pid && msgq.mesg_type == lem->team)
	{
		if (lem->v)
			ft_printf("recv chase -> %d %d\n", msgq.mes.y, msgq.mes.x);
		move_to_target(lem, msgq.mes.y, msgq.mes.x);
	}
	else if (check_nearly_ennemi(lem, &y, &x))
	{
		send_target_msg(lem, y, x);
		if (lem->v)
			ft_printf("send chase -> %d %d\n", y, x);
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
			if (lem->v)
				ft_printf("Your team win\n");
			return ;
		}
		if (lem->alive && check_equality(lem))
		{
			free_space(lem);
			if (lem->v)
				ft_printf("Equality\n");
			return ;
		}
		if (lem->alive && check_if_encircled(lem))
		{
			free_space(lem);
			lem->alive = 0;
			if (lem->v)
				ft_printf("You are dead\n");
			if (!lem->creator)
				return ;
		}
		if (lem->alive)
			player_process(lem);
		if (lem->creator || lem->c)
			display_map(lem);
		if (lem->creator && !lem->alive && check_if_empty(lem))
				return ;
		sleep(SLEEP_VALUE);
	}
}
