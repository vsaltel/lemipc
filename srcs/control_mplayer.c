#include "lemipc.h"

enum control {UP, DOWN, LEFT, RIGHT};

int		move_player(t_lem *lem, int move)
{
	if (move == RIGHT && !(lem->shm->area[lem->y][lem->i + 1]) && (lem->i + 1) < MAP_SIZE)
	{
			lem->shm->area[lem->y][lem->i] = 0;
			lem->shm->area[lem->y][++(lem->i)] = lem->team;
			return (0);
	}
	else if (move == LEFT && !(lem->shm->area[lem->y][lem->i - 1]) && (lem->i - 1) >= 0)
	{
			lem->shm->area[lem->y][lem->i] = 0;
			lem->shm->area[lem->y][--(lem->i)] = lem->team;
			return (0);
	}
	else if (move == UP && !(lem->shm->area[lem->y - 1][lem->i]) && (lem->y - 1) >= 0)
	{
			lem->shm->area[lem->y][lem->i] = 0;
			lem->shm->area[--(lem->y)][lem->i] = lem->team;
			return (0);
	}
	else if (move == DOWN && !(lem->shm->area[lem->y + 1][lem->i]) && (lem->y + 1) < MAP_SIZE)
	{
			lem->shm->area[lem->y][lem->i] = 0;
			lem->shm->area[++(lem->y)][lem->i] = lem->team;
			return (0);
	}
	return (1);
}

void	control_player(t_lem *lem)
{
	char	buf[BUFFER_SIZE];
	int		ret;

	while ((ret = read(0, buf, BUFFER_SIZE)) > -1)
	{
		buf[ret] = '\0';
		ft_printf("len = %d\n", ft_strlen(buf));
		if (!ft_strncmp("\e[C", buf, 3) && !move_player(lem, RIGHT))
			break;
		if (!ft_strncmp("\e[D", buf, 3) && !move_player(lem, LEFT))
			break;
		if (!ft_strncmp("\e[1;2A", buf, 6) && !move_player(lem, UP))
			break;
		if (!ft_strncmp("\e[1;2B", buf, 6) && !move_player(lem, DOWN))
			break;
		ft_dprintf(2, "lemipc: bad input or impossible move\n");
	}
	if (ret <= 0)
		exit_free(lem);
}

int		init_shell_input(t_lem *lem)
{
	struct termios term;

	if (!isatty(0))
		return (1);
	tcgetattr(0, &term);
	lem->prev_term = term;
	term.c_lflag &= ~(ICANON | ECHO | IEXTEN | OPOST);
	term.c_cc[VMIN] = 3;
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
