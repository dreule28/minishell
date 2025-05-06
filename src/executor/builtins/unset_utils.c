/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:24 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/06 10:06:37 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (-1);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (-1);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

int	unset_loop(t_cmd_node *cmd_node, t_env_node **env_node,
		t_env_list *env_list, int count)
{
	t_env_node	*prev_env_node;
	t_env_node	*curr_env_node;

	prev_env_node = NULL;
	curr_env_node = *env_node;
	while (curr_env_node)
	{
		if (ft_strcmp(cmd_node->cmd[count], curr_env_node->type) == 0)
		{
			if (prev_env_node == NULL)
				env_list->head = curr_env_node->next;
			else
				prev_env_node->next = curr_env_node->next;
			return (-1);
		}
		prev_env_node = curr_env_node;
		curr_env_node = curr_env_node->next;
		*env_node = curr_env_node;
	}
	return (0);
}
