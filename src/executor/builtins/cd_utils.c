/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:09 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 17:52:10 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_argument(char **argument)
{
	int	count;

	count = 0;
	while (argument[count] != NULL)
		count++;
	return (count);
}

char	*get_home(t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("HOME", env_node->type) == 0)
			return (gc_strdup(env_node->value));
		env_node = env_node->next;
	}
	return (NULL);
}

char	*get_old_pwd(t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("OLDPWD", env_node->type) == 0)
			return (gc_strdup(env_node->value));
		env_node = env_node->next;
	}
	return (NULL);
}

void	update_old_pwd(char *old_pwd, t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("OLDPWD", env_node->type) == 0)
		{
			env_node->value = gc_strdup(old_pwd);
			return ;
		}
		env_node = env_node->next;
	}
}

void	update_new_pwd(char *new_pwd, t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("PWD", env_node->type) == 0)
		{
			env_node->value = new_pwd;
			return ;
		}
		env_node = env_node->next;
	}
}
