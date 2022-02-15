#include "libft.h"

int	ft_mret(char *str, int ret)
{
	write(2, str, ft_strlen(str));
	return (ret);
}
