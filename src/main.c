#include "minishell.h"

int	main(void)
{
	from_executor();
	from_parser();
	printf("This is a test from main\n");
	return (0);
}
