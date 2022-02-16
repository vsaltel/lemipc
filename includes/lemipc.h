#ifndef LEMIPC_H
# define LEMIPC_H

# include <sys/mman.h>
# include <signal.h>

# include "libft.h"

# define MAX_PLAYER	10
# define MAP_SIZE	6
# define SEM_NAME	"lemipc_sem"

/*
union semun
{
	int					val;
	struct semid_ds		*buf;
	unsigned short int	*array;
	struct seminfo		*%%__%%buf;
};
*/

typedef struct s_shm
{
	int		area[MAP_SIZE][MAP_SIZE];
	int		player_round;
	int		game_state;
}				t_shm;

typedef struct s_lem
{
	t_shm	*shm;
	int		pids[MAX_PLAYER - 1];
	int		semid;
	int		main_player;	
	int		nb_player;
	int		player_id;
	int		pid;
	int		team;
	int		y;
	int		i;
}				t_lem;

extern t_lem	lem;

/* create_game.c */
int		create_game(t_lem *lem);

/* utils.c */
void	init_lem(t_lem *lem);
void	free_shm(t_shm *shm);
void	*create_shared_memory(size_t size);
void	catch_sigint(int signal);

/* display.c */
void	display_map(t_lem *lem);

/* game_process.c */
int		main_player(t_lem *lem);
int		other_player(t_lem *lem);
void	player_process(t_lem *lem);

/* semaphore.c */
int		sem_destroy(int semid);
int		sem_alloc(void);
int		sem_init(int semid);
int		sem_wait(int semid);
int		sem_post(int semid);

#endif
