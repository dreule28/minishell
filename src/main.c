#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*test_str;

	(void)argc;
	(void)argv;
	(void)env;
	gc_init();
	while (1)
	{
		test_str = readline("2, 3 Years dagestan forget > ");
		if (test_str)
		{
			if (ft_strncmp(test_str, "exit", 4) == 0)
			{
				free(test_str);
				break ;
			}
			printf("prompt : %s\n", test_str);
			free(test_str);
		}
	}
	clean_up();
	return (0);
}
