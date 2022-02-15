#include "lemipc.h"

void	player_process(t_lem *lem)
{
	ft_printf("pos : y=%d i=%d\n", lem->y, lem->i);	
}

void	other_player(t_lem *lem)
{
	//while (1)
		player_process(lem);	
}

void	main_player(t_lem *lem)
{
	//while (1)
	//{
		display_map(lem);
		player_process(lem);
	//}
}
