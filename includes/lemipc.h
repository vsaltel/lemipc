#ifndef LEMIPC_H
# define LEMIPC_H

# include <errno.h>
# include <stdio.h>
# include <sys/mman.h>
# include <signal.h>
# include <time.h>
# include <termios.h>

# include "libft.h"

# define BUFFER_SIZE	1024
# define MAP_SIZE		10
# define NB_TEAM		3
# define WAIT_PLAYER	6
# define SLEEP_VALUE	1

enum control {UP, LEFT, DOWN, RIGHT};

union semun_u
{
	int				val;
	struct semid_ds	*buf;
	unsigned short	*array;
	struct seminfo	*__buf;
};

typedef struct s_msgt
{
	int		pid;
	int		x;
	int		y;
}				t_msgt;

typedef struct s_msgq
{
    long	mesg_type;
	t_msgt	mes;
}				t_msgq;

typedef struct s_shm
{
	int		area[MAP_SIZE][MAP_SIZE];
}				t_shm;

typedef struct s_lem
{
	t_shm			*shm;
	struct termios	prev_term;
    int				msgqid;
	int				semid;
	int				shmid;
	int				sem_taken;
	int				alive;
	int				pid;
	int				creator;
	int				nb_team;
	int				team;
	int				y;
	int				x;
	int				v;
	int				c;
}				t_lem;

extern t_lem	lem;

/* create_game.c */
int		create_game(t_lem *lem);

/* utils.c */
void	free_space(t_lem *lem);
void	init_lem(t_lem *lem);
void	catch_sigint(int signal);
void	exit_free(t_lem *lem);

/* display.c */
void	display_map(t_lem *lem);

/* game_process.c */
void	player(t_lem *lem);

/* semaphore.c */
int		sem_destroy(t_lem *lem);
int		sem_alloc(t_lem *lem);
int		sem_init(int semid);
int		sem_wait(int semid);
int		sem_post(int semid);

/* control_mplayer.c */
int		move_player(t_lem *lem, int move);
void	move_to_target(t_lem *lem, int dy, int dx);
void	control_player(t_lem *lem);
int		init_shell_input(t_lem *lem);
void	restore_shell_input(t_lem *lem);

/* game_utils.c */
int		check_equality(t_lem *lem);
int		check_nearly_ennemi(t_lem *lem, int *dy, int *dx);
int		check_nb_player_team(t_lem *lem, int team);
int		check_if_last_team(t_lem *lem);
int		check_nb_player(t_lem *lem);
int		check_if_empty(t_lem *lem);
int		check_if_encircled(t_lem *lem);

/* msgq.c */
int		join_msgq(t_lem *lem);
int		free_msgq(t_lem *lem);
int		send_target_msg(t_lem *lem, int y, int x);
int		receive_message(t_lem *lem, t_msgq *msgq);

/* shm.c */
void	*create_shared_memory(t_lem *lem);
int		free_shm(t_lem *lem);
void	init_shm(t_lem *lem);

#endif
