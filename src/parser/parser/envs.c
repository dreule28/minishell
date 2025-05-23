/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:28:01 by dreule            #+#    #+#             */
/*   Updated: 2025/05/06 16:17:23 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_list	*init_env_list(void)
{
	t_env_list	*new_list;

	new_list = ft_malloc(sizeof(t_env_list), 1);
	if (!new_list)
		return (NULL);
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->size = 0;
	return (new_list);
}

void	add_env_node(t_env_list *env_list, char *type, char *value)
{
	t_env_node	*new_node;

	new_node = ft_malloc(sizeof(t_env_node), 1);
	if (!new_node)
		return ;
	new_node->next = NULL;
	new_node->type = gc_strdup(type);
	new_node->value = gc_strdup(value);
	if (!env_list->head)
		env_list->head = new_node;
	else
		env_list->tail->next = new_node;
	env_list->tail = new_node;
	env_list->size++;
}

char	*find_equal_sign(char **env, int i, char **value)
{
	char	*type;
	char	*equal_pos;
	int		len;

	equal_pos = ft_strchr(env[i], '=');
	if (!equal_pos)
		return (gc_strdup(env[i]));
	len = equal_pos - env[i];
	type = gc_substr(env[i], 0, len);
	*value = gc_strdup(equal_pos + 1);
	return (type);
}

t_env_list	*get_envs(char **env)
{
	t_env_list	*env_list;
	int			i;
	char		*type;
	char		*value;

	i = 0;
	env_list = init_env_list();
	while (env[i])
	{
		type = find_equal_sign(env, i, &value);
		if (ft_strcmp(type, "OLDPWD") == 0)
		{
			i++;
			add_env_node(env_list, "OLDPWD", "");
		}
		else if (type)
			add_env_node(env_list, type, value);
		i++;
	}
	add_shell_level(env_list);
	return (env_list);
}

void	add_shell_level(t_env_list *env_list)
{
	int			temp;
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("SHLVL", env_node->type) == 0)
		{
			temp = ft_atoi(env_node->value);
			temp++;
			env_node->value = gc_itoa(temp);
			return ;
		}
		env_node = env_node->next;
	}
}
