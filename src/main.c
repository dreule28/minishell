/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:53:51 by dreule            #+#    #+#             */
/*   Updated: 2025/05/06 17:03:00 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_list	*setup_env_list(void)
{
	t_env_list	*env_list;
	char		*pwd;

	pwd = getcwd(NULL, 0);
	gc_add(pwd);
	env_list = init_env_list();
	add_env_node(env_list, "SHLVL", "2");
	add_env_node(env_list, "PWD", pwd);
	add_env_node(env_list, "OLDPWD", gc_strdup(""));
	return (env_list);
}

t_env_list	*initialize_shell(char **env)
{
	t_env_list	*env_list;

	gc_init();
	if (!env || !env[0])
		env_list = setup_env_list();
	else
		env_list = get_envs(env);
	if (!env_list)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		*exit_code() = 1;
		return (NULL);
	}
	start_signals();
	return (env_list);
}

char	*get_prompt(void)
{
	char	*prompt;
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		prompt = readline("minishell$ ");
		if (prompt && *prompt)
			add_history(prompt);
		return (prompt);
	}
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	prompt = ft_strtrim(line, "\n");
	free(line);
	return (prompt);
}

int	process_command(char *prompt, t_env_list *env_list)
{
	t_token_list	*tk_list;
	t_cmd_list		*cmd_list;

	if (!prompt)
		return (0);
	if (*prompt == '\0')
		return (*exit_code() = 0, 1);
	gc_add(prompt);
	tk_list = lexer(prompt);
	if (!tk_list)
		return (1);
	tk_list = process_token_list(tk_list);
	if (!tk_list)
		return (1);
	segment_tokens(tk_list, env_list);
	cmd_list = token_to_cmd(tk_list);
	if (!cmd_list)
		return (1);
	if (execute(env_list, cmd_list) == 0)
	{
		delete_tmp_files("/tmp");
		return (0);
	}
	delete_tmp_files("/tmp");
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	t_env_list	*env_list;
	char		*prompt;
	int			keep_running;

	(void)argc;
	(void)argv;
	env_list = initialize_shell(env);
	if (!env_list)
		return (clean_up(), 1);
	while (1)
	{
		prompt = get_prompt();
		if (!prompt)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", 2);
			break ;
		}
		keep_running = process_command(prompt, env_list);
		if (!keep_running)
			break ;
	}
	clean_up();
	return (*exit_code());
}
