#include "lemipc.h"

int		move_player(t_lem *lem, int move)
{
	if (move == RIGHT && (lem->x + 1) < MAP_SIZE && !(lem->shm->area[lem->y][lem->x + 1]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[lem->y][++(lem->x)] = lem->team;
			return (0);
	}
	else if (move == LEFT && (lem->x - 1) >= 0 && !(lem->shm->area[lem->y][lem->x - 1]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[lem->y][--(lem->x)] = lem->team;
			return (0);
	}
	else if (move == UP && (lem->y - 1) >= 0 && !(lem->shm->area[lem->y - 1][lem->x]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[--(lem->y)][lem->x] = lem->team;
			return (0);
	}
	else if (move == DOWN && (lem->y + 1) < MAP_SIZE && !(lem->shm->area[lem->y + 1][lem->x]))
	{
			lem->shm->area[lem->y][lem->x] = 0;
			lem->shm->area[++(lem->y)][lem->x] = lem->team;
			return (0);
	}
	return (1);
}

void	move_to_target(t_lem *lem, int dy, int dx)
{
	int ret;

	ft_printf("move to target : %d %d\n", dy, dx);
	ret = 1;
	if (dy > lem->y)
	{
		ft_printf("go down\n");
		ret = move_player(lem, DOWN);
	}
	else if (dy < lem->y)
	{
		ft_printf("go up\n");
		ret = move_player(lem, UP);
	}
	else if (dx > lem->x)
	{
		ft_printf("go right\n");
		ret = move_player(lem, RIGHT);
	}
	else if (dx < lem->x)
	{
		ft_printf("go left\n");
		ret = move_player(lem, LEFT);
	}
	if (ret)
		if (move_player(lem, RIGHT))
			if (lem->x + 1 != dx)
				move_player(lem, LEFT);
}

void	control_player(t_lem *lem)
{
	char	buf[BUFFER_SIZE];
	int		ret;

	ft_printf("Your turn\n");
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
			exit_free(lem);
		ft_dprintf(2, "lemipc: bad input or impossible move\n");
	}
	if (ret <= 0)
		exit_free(lem);
}

/*
int		init_shell_input(t_lem *lem)
{
	struct termios term;

	if (!isatty(0))
		return (1);
	tcgetattr(0, &term);
	lem->prev_term = term;
	term.c_lflag &= ~(ICANON | ECHO | IEXTEN | OPOST);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &term))
		return (2);
	return (0);
}

void		restore_shell_input(t_lem *lem)
{
	if (!isatty(0))
		return ;
	tcsetattr(0, TCSANOW, &(lem->prev_term));
}
*/
