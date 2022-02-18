#include "lemipc.h"

static int		check_if_encircled(t_lem *lem)
{
	int	count;
	int	other_team;

	other_team = (lem->team == 1) ? 2 : 1;
	count = 0;
	if ((lem->i + 1) < MAP_SIZE && (lem->shm->area[lem->y][lem->i + 1]) == other_team)
		count++;
	if ((lem->i - 1) >= 0 && (lem->shm->area[lem->y][lem->i - 1]) == other_team)
		count++;
	if ((lem->y - 1) >= 0 && (lem->shm->area[lem->y - 1][lem->i]) == other_team)
		count++;
	if ((lem->y + 1) < MAP_SIZE && (lem->shm->area[lem->y + 1][lem->i]) == other_team)
		count++;
	if (count >= 2)
	{
		lem->shm->area[lem->y][lem->i] = 0;
		lem->alive = 0;
		return (1);
	}
	return (0);
}

static int		player_turn(t_lem *lem)
{
	sem_wait(lem->semid);
	if (!lem->shm->game_state)
	{
		sem_post(lem->semid);
		return (1);
	}
	if (check_if_encircled(lem))
	{
		sem_post(lem->semid);
		return (2);
	}
	if (lem->pid)
		control_player(lem);
	else
		ft_printf("id=%d pos : y=%d i=%d\n", lem->player_id, lem->y, lem->i);
	sem_post(lem->semid);
	send_turn_msg(lem, lem->player_id + 1);
	return (0);
}

void	player(t_lem *lem)
{
	t_msgq	msgq;
	int		first;

	first = lem->pid ? 1 : 0;
	while (1)
	{
		ft_printf("there\n");
		if (!first && receive_message(lem, &msgq))
			break;
		if (msgq.mesg_type == 1 || first)
		{
			if (lem->pid)
				display_map(lem);
			if ((lem->alive && msgq.mes.nb == lem->player_id) || first)
				if (player_turn(lem))
					break;
		}
		if (msgq.mesg_type == 2)
		{
			if (msgq.mes.nb < lem->player_id)
				lem->player_id--;
		}
		first = 0;
	}
}
