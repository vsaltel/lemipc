#include "lemipc.h"

t_lem	lem;

int	main(void)
{
	int		ret;

	init_lem(&lem);
	if ((ret = create_game(&lem)))
		return (ret);
	return (0);
}
