#include "minishell.h"

void	error_message(char *str)
{
	ft_putstr_fd(str, 2);
	return ;
}
