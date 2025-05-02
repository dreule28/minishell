/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_cd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:19 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 17:52:20 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir_names(char *old_dir, char *home_dir, char *dir_name)
{
	if (old_dir == NULL)
	{
		*exit_code() = 1;
		ft_putstr_fd("OLDPWD not set\n", 2);
		return (-1);
	}
	if (home_dir == NULL)
	{
		*exit_code() = 1;
		ft_putstr_fd("HOME not set\n", 2);
		return (-1);
	}
	if (chdir(dir_name) != 0)
	{
		*exit_code() = 1;
		ft_putstr_fd("No such a file or directory\n", 2);
		return (-1);
	}
	return (0);
}

void	update_pwds(t_cmd_node *cmd_node, t_env_list *env_list, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	gc_add(new_pwd);
	update_old_pwd(old_pwd, env_list);
	update_new_pwd(new_pwd, env_list);
	if (cmd_node->cmd[1] && ft_strcmp(cmd_node->cmd[1], "-") == 0)
	{
		ft_putstr_fd(new_pwd, 1);
		ft_putstr_fd("\n", 1);
	}
}
