#include "lemipc.h"

void	player_process(t_lem *lem)
{
	ft_printf("id=%d pos : y=%d i=%d\n", lem->player_id, lem->y, lem->i);	
}

int		other_player(t_lem *lem)
{
	while (1)
	{
		sem_wait(lem->semid);
		if (!lem->shm->game_state)
			break;
		if (lem->shm->player_round == lem->player_id)
		{
			player_process(lem);	
			if (++lem->shm->player_round > lem->nb_player)
				lem->shm->player_round = 1;
		}
		sem_post(lem->semid);
	}
	return (0);
}

int		main_player(t_lem *lem)
{
	signal(SIGINT, &catch_sigint);
	while (1)
	{
		sem_wait(lem->semid);
		if (!lem->shm->game_state)
			break;
		if (lem->shm->player_round == lem->player_id)
		{
			display_map(lem);
			ft_printf("main=%d pos : y=%d i=%d\n", lem->player_id, lem->y, lem->i);	
			control_player(lem);
			if (++lem->shm->player_round > lem->nb_player)
				lem->shm->player_round = 1;
		}
		//lem->shm->game_state = 0;
		sem_post(lem->semid);
	}
	sem_destroy(lem->semid);
	return (0);
}
