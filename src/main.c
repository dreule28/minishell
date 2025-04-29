#include "minishell.h"

int	*exit_code(void)
{
	static int	exit_code;

	return (&exit_code);
}

t_env_list 	*setup_env_list()
{
	t_env_list *env_list;
	char *pwd;

	pwd = getcwd(NULL, 0);
	gc_add(pwd);
	env_list = init_env_list();
	add_env_node(env_list, "SHLVL", "2");
	add_env_node(env_list, "PWD", pwd);
	add_env_node(env_list, "OLDPWD", gc_strdup(""));
	return(env_list);
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
	if(env[0] == NULL)
		env_list = setup_env_list();
	else
		env_list = get_envs(env);
	start_signals();
	while (1)
	{
		if (isatty(fileno(stdin)))
		{
			prompt = readline("2, 3 Years dagestan forget > ");
			if (prompt && ft_strlen(prompt) > 0)
            	add_history(prompt);
		}
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			if(!line)
				break ; 
			prompt = ft_strtrim(line, "\n");
			free(line);
		}
		if (!prompt)
		{
			ft_putstr_fd("exit\n", 2);
			clean_up();
			exit(*exit_code());
		}
		if (*prompt == '\0')
		{
			*exit_code() = 0;
			continue;
		}
		if (prompt)
		{
			gc_add(prompt);

			// if (ft_strlen(prompt) > 0)
			// 	add_history(prompt);
			tk_list = lexer(prompt);
			if (!tk_list)
				return (-1);
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