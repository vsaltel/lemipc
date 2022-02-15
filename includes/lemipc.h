#ifndef LEMIPC_H
# define LEMIPC_H

# include <sys/mman.h>

# include "libft.h"

# define MAX_PLAYER	10
# define MAP_SIZE	6

typedef struct s_shm
{
	int		area[MAP_SIZE][MAP_SIZE];
}				t_shm;

typedef struct s_lem
{
	t_shm	*shm;
	int		main_player;	
	int		nb_player;
	int		pid;
	int		team;
	int		y;
	int		i;
}				t_lem;

/* create_game.c */
int		create_game(t_lem *lem);

/* utils.c */
void	init_lem(t_lem *lem);
void	free_shm(t_shm *shm);
void	*create_shared_memory(size_t size);


/* display.c */
void	display_map(t_lem *lem);

/* game_process.c */
void	main_player(t_lem *lem);
void	other_player(t_lem *lem);
void	player_process(t_lem *lem);

#endif
