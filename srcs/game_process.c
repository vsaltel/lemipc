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
		sem_wait(semid);
		lem->shm->area[lem->y][lem->i] = 0;
		sem_post(semid);
		lem->alive = 0;
		return (1);
	}
	return (0);
}

void	player(t_lem *lem)
{
	//t_msgq	msgq;

	while (1)
	{
		display_map(lem);
		if (check_if_encircled(lem))
			return ;
		sleep(2);	
	}
}
