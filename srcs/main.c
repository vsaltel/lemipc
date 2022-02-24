#include "lemipc.h"

t_lem	lem;

void	parse_arguments(t_lem *lem, int argc, char **argv)
{
	int i;

	i = 0;
	while (++i < argc)
	{
		if (!ft_strcmp(argv[i], "-v"))
			lem->v = 1;
		if (!ft_strcmp(argv[i], "-c"))
			lem->c = 1;
	}
}

int	main(int argc, char **argv)
{
	int		ret;

	init_lem(&lem);
	parse_arguments(&lem, argc, argv);
	if ((ret = create_game(&lem)))
		return (ret);
	return (0);
}
