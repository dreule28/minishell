/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:07 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/06 16:10:56 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(t_cmd_node *cmd_node)
{
	int	i;
	int	no_flag;
	int	print_space;

	i = 1;
	no_flag = 1;
	print_space = 0;
	while (cmd_node->cmd[i] && is_all_flag(cmd_node->cmd[i], 'n')
		&& cmd_node->cmd[i][0] == '-')
	{
		no_flag = 0;
		i++;
	}
	while (cmd_node->cmd[i])
	{
		if (print_space)
			ft_putstr_fd(" ", 1);
		ft_putstr_fd(cmd_node->cmd[i], 1);
		print_space = 1;
		i++;
	}
	if (no_flag)
		ft_putstr_fd("\n", 1);
	*exit_code() = 0;
}

void	builtin_cd(t_cmd_node *cmd_node, t_env_list *env_list)
{
	char	*old_dir;
	char	*home_dir;
	char	*dir_name;
	char	*old_pwd;

	old_dir = gc_strdup("");
	home_dir = gc_strdup("");
	dir_name = gc_strdup("");
	old_pwd = getcwd(NULL, 0);
	gc_add(old_pwd);
	if (cmd_node->cmd[1] == NULL || ft_strcmp(cmd_node->cmd[1], "~") == 0)
	{
		home_dir = get_home(env_list);
		dir_name = home_dir;
	}
	else if (ft_strcmp(cmd_node->cmd[1], "-") == 0)
	{
		old_dir = get_old_pwd(env_list);
		dir_name = old_dir;
	}
	else
		dir_name = cmd_node->cmd[1];
	if (check_dir_names(old_dir, home_dir, dir_name) == -1)
		return ;
	update_pwds(cmd_node, env_list, old_pwd);
}

void	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd != NULL)
	{
		ft_putstr_fd(pwd, 1);
		free(pwd);
		ft_putstr_fd("\n", 1);
	}
	else
		ft_putstr_fd("get_cwd function failed (returned NULL)\n", 2);
	*exit_code() = 0;
}

int	builtin_exit(t_cmd_node *cmd_node)
{
	int	exit_val;
	int	check;

	check = 0;
	if (cmd_node->cmd[0] || (cmd_node->cmd[0] && cmd_node->cmd[1]))
	{
		if (cmd_node->cmd[0] && cmd_node->cmd[1])
		{
			ft_putstr_fd("exit\n", 2);
			check = check_exit_arguments(cmd_node);
			if (check == -1)
				return (1);
			else if (check == 1)
				return (0);
			exit_val = ft_atoi(cmd_node->cmd[1]);
			*exit_code() = exit_val % 256;
			return (1);
		}
		else if (cmd_node->cmd[0])
			return (1);
	}
	return (0);
}
