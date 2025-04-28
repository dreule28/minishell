#include "minishell.h"

int	*exit_code(void)
{
	static int	exit_code;

	return (&exit_code);
}

int main(int argc, char **argv, char **env)
{
	char			*prompt;
	t_token_list	*tk_list;
	t_cmd_list		*cmd_list;
	t_env_list		*env_list;
	(void)argc;
	(void)argv;


	gc_init();
	env_list = get_envs(env);
	start_signals();
	while (1)
	{
		prompt = readline("2, 3 Years dagestan forget > ");
		if (!prompt)
		{
			ft_putstr_fd("exit\n", 2);
			clean_up();
			exit(0);
		}
		if (*prompt == '\0')
		{
			*exit_code() = 0;
			continue;
		}
		if (prompt)
		{
			gc_add(prompt);

			if (ft_strlen(prompt) > 0)
				add_history(prompt);
			tk_list = lexer(prompt);
			if (!tk_list)
				continue;
			
				tk_list = process_token_list(tk_list);
				if (!tk_list)
					continue;

			segment_tokens(tk_list, env_list);
			cmd_list = token_to_cmd(tk_list);
			if (!cmd_list)
				continue;

			if(execute(env_list, cmd_list) == 0)
			{
				delete_tmp_files("tmp");
				break;
			}
			delete_tmp_files("tmp");
		}
	}
	clean_up();
	exit(*exit_code());
	return (0);
	}

// dorker valgrind --leak-check=full  --show-leak-kinds=all --suppressions=sub.sub --track-fds=yes ./minishell