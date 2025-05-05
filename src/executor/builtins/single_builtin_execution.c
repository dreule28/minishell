/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_builtin_execution.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:22 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/05 17:33:23 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	single_builtin_execution(t_cmd_node *cmd_node, t_env_list *env_list)
{
	char	*lower_str;
	int		saved_stdin;
	int		saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (file_redirecting(cmd_node) == -1)
	{
		reset_redir(saved_stdin, saved_stdout);
		return (1);
	}
	lower_str = turn_lower(cmd_node->cmd[0]);
	if (search_builtin(cmd_node, env_list, lower_str))
	{
		reset_redir(saved_stdin, saved_stdout);
		return (*exit_code());
	}
	reset_redir(saved_stdin, saved_stdout);
	return (0);
}

int	search_return_code(t_cmd_node *cmd_node)
{
	if (cmd_node->cmd[1] == NULL)
	{
		clean_up();
		return (*exit_code());
	}
	if (ft_isnum_exit(cmd_node->cmd[1]) == 0 && cmd_node->cmd[2] != NULL)
	{
		clean_up();
		return (*exit_code());
	}
	clean_up();
	return (*exit_code());
}

int	ft_isnum_exit(char *str)
{
	int	count;
	int	has_digit;

	count = 0;
	has_digit = 0;
	while (str[count] == '+' || str[count] == '-')
		count++;
	while (str[count] != '\0')
	{
		if (!(str[count] >= '0' && str[count] <= '9'))
			return (-1);
		has_digit = 1;
		count++;
	}
	if (!has_digit)
		return (-1);
	return (0);
}

bool	search_builtin(t_cmd_node *cmd_node, t_env_list *env_list, char *str)
{
	if (!ft_strcmp(str, "pwd"))
		builtin_pwd();
	else if (!ft_strcmp(str, "export"))
		builtin_export(cmd_node, env_list);
	else if (!ft_strcmp(str, "cd"))
		builtin_cd(cmd_node, env_list);
	else if (!ft_strcmp(str, "unset"))
		builtin_unset(cmd_node, env_list);
	else if (!ft_strcmp(str, "env"))
		builtin_env(cmd_node, env_list);
	else if (!ft_strcmp(str, "echo"))
		builtin_echo(cmd_node);
	else if (!ft_strcmp(str, "exit"))
	{
		builtin_exit(cmd_node);
		clean_up();
		return (true);
	}
	else
		ft_putstr_fd("Command not found\n", 2);
	return (false);
}

void	reset_redir(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
