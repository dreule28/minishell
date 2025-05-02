/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:13 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 17:52:14 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_cmd_node *cmd_node, t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	if (cmd_node->cmd[1] != NULL)
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(cmd_node->cmd[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return ;
	}
	while (env_node != NULL)
	{
		if (env_node->value != NULL && env_node->value[0] != '\0')
		{
			ft_putstr_fd(env_node->type, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(env_node->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env_node = env_node->next;
	}
}

void	builtin_export(t_cmd_node *cmd_node, t_env_list *env_list)
{
	if (cmd_node->cmd[1] == NULL)
		print_export_variables(env_list);
	else
		export_variable(cmd_node, env_list);
}

void	builtin_unset(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int			count;
	int			invalid_found;
	t_env_node	*env_node;

	env_node = env_list->head;
	invalid_found = 0;
	count = 1;
	while (cmd_node->cmd[count] != NULL)
	{
		if (is_valid_identifier(cmd_node->cmd[count]) == -1)
		{
			invalid_found = 1;
			ft_putstr_fd("not a valid identifier\n", 2);
			count++;
			continue ;
		}
		while (env_node != NULL)
			if (unset_loop(cmd_node, &env_node, env_list, count) == -1)
				break ;
		count++;
	}
	*exit_code() = invalid_found;
}
