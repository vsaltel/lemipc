#include "lemipc.h"

int	main(void)
{
	t_lem	lem;
	int		ret;

	init_lem(&lem);
	if ((ret = create_game(&lem)))
		return (ret);
	return (0);
}
