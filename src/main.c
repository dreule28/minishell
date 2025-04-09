#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*prompt;
	// int		exit_code;
	(void)argc;
	(void)argv;
	(void)env;
	// (void)exit_code;
	gc_init();
	while (1)
	{
		prompt = readline("2, 3 Years dagestan forget > ");
		if (prompt)
		{
			gc_add(prompt); 								// adds the prompt string to the garbage collector
			// fromp_parser(prompt);
			execute(env);
			// history_add("");
			if (ft_strncmp(prompt, "exit", 4) == 0)
				break ;
			clean_up();
		}
	}
	clean_up(); 
	return (0);
}
