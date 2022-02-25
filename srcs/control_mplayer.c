#include "lemipc.h"

int		move_player(t_lem *lem, int move)
{
	int ret;

	ret = 1;
	sem_wait(lem->semid);
	if (move == RIGHT && (lem->x + 1) < MAP_SIZE && !(lem->shm->area[lem->y][lem->x + 1]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[lem->y][++(lem->x)] = lem->team;
			ret = 0;
	}
	else if (move == LEFT && (lem->x - 1) >= 0 && !(lem->shm->area[lem->y][lem->x - 1]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[lem->y][--(lem->x)] = lem->team;
			ret = 0;
	}
	else if (move == UP && (lem->y - 1) >= 0 && !(lem->shm->area[lem->y - 1][lem->x]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[--(lem->y)][lem->x] = lem->team;
			ret = 0;
	}
	else if (move == DOWN && (lem->y + 1) < MAP_SIZE && !(lem->shm->area[lem->y + 1][lem->x]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[++(lem->y)][lem->x] = lem->team;
			ret = 0;
	}
	sem_post(lem->semid);
	return (ret);
}

void	move_to_target(t_lem *lem, int dy, int dx)
{
	static int i;

	if (!i)
	{
		if (dy > lem->y)
		{
			if (move_player(lem, DOWN))
				if (move_player(lem, LEFT))
					move_player(lem, RIGHT);
		}
		else if (dx > lem->x)
		{
			if (move_player(lem, RIGHT))
				if (move_player(lem, UP))
					move_player(lem, DOWN);
		}
		else if (dy < lem->y)
		{
			if (move_player(lem, UP))
				if (move_player(lem, RIGHT))
					move_player(lem, LEFT);
		}
		else if (dx < lem->x)
		{
			if (move_player(lem, LEFT))
				if (move_player(lem, DOWN))
					move_player(lem, UP);
		}
		i++;
	}
	else
	{
		if (dx < lem->x)
		{
			if (move_player(lem, LEFT))
				if (move_player(lem, DOWN))
					move_player(lem, UP);
		}
		else if (dy < lem->y)
		{
			if (move_player(lem, UP))
				if (move_player(lem, RIGHT))
					move_player(lem, LEFT);
		}
		else if (dx > lem->x)
		{
			if (move_player(lem, RIGHT))
				if (move_player(lem, UP))
					move_player(lem, DOWN);
		}
		else if (dy > lem->y)
		{
			if (move_player(lem, DOWN))
				if (move_player(lem, LEFT))
					move_player(lem, RIGHT);
		}
		i = 0;
	}
}

void	control_player(t_lem *lem)
{
	char	buf[BUFFER_SIZE];
	int		ret;

	while (1)
	{
		ret = read(0, buf, BUFFER_SIZE);
		if (ret == -1)
			exit_free(lem);
		buf[ret] = '\0';
		if (!ft_strncmp("\e[C", buf, 3) && !move_player(lem, RIGHT))
			break;
		else if (!ft_strncmp("\e[D", buf, 3) && !move_player(lem, LEFT))
			break;
		else if (!ft_strncmp("\e[A", buf, 3) && !move_player(lem, UP))
			break;
		else if (!ft_strncmp("\e[B", buf, 3) && !move_player(lem, DOWN))
			break;
		else if (!ft_strncmp("\x03", buf, 1))
			kill(lem->pid, SIGINT);
	}
	if (ret <= 0)
		exit_free(lem);
}

int		init_shell_input(t_lem *lem)
{
	struct termios term;

	ft_printf("there\n");
	if (!isatty(0))
	{
		lem->c = 0;
		return (1);
	}
	tcgetattr(0, &term);
	lem->prev_term = term;
	term.c_lflag &= ~(ICANON | ECHO | IEXTEN | OPOST);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &term))
	{
		lem->c = 0;
		return (2);
	}
	lem->c++;
	return (0);
}

void		restore_shell_input(t_lem *lem)
{
	if (!isatty(0))
		return ;
	tcsetattr(0, TCSANOW, &(lem->prev_term));
}
