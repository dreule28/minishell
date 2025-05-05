/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:51:51 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 18:06:32 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_converter_loop(t_env_node **env_node, char **converted_env_list,
		int *count)
{
	char	*tmp;
	char	*env_str;

	tmp = ft_strjoin((*env_node)->type, "=");
	env_str = gc_strjoin(tmp, (*env_node)->value);
	free(tmp);
	if (!env_str)
	{
		ft_putstr_fd("Error: memory allocation failed\n", 2);
		return (-1);
	}
	converted_env_list[(*count)] = env_str;
	*env_node = (*env_node)->next;
	(*count)++;
	return (0);
}

char	**env_converter(t_env_list *env_list)
{
	int			count;
	t_env_node	*env_node;
	char		**converted_env_list;

	count = 0;
	env_node = env_list->head;
	count = env_lenght(env_list);
	converted_env_list = ft_malloc(count + 1, sizeof(char *));
	if (!converted_env_list)
		return (ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
	env_node = env_list->head;
	count = 0;
	while (env_node)
		if (env_converter_loop(&env_node, converted_env_list, &count) == -1)
			return (NULL);
	converted_env_list[count] = NULL;
	return (converted_env_list);
}

bool	env_converter_export_loop(t_env_node *env_node,
		char **converted_env_list, int *count)
{
	char	*tmp;
	char	*env_str;

	tmp = ft_strjoin(env_node->type, "=");
	env_str = ft_strjoin(tmp, "\"");
	free(tmp);
	tmp = ft_strjoin(env_str, env_node->value);
	free(env_str);
	env_str = gc_strjoin(tmp, "\"");
	free(tmp);
	if (!env_str)
	{
		ft_putstr_fd("Error: memory allocation failed\n", 2);
		return (true);
	}
	converted_env_list[(*count)++] = env_str;
	return (false);
}

char	**env_converter_export(t_env_list *env_list)
{
	int			count;
	t_env_node	*env_node;
	char		**converted_env_list;

	count = 0;
	env_node = env_list->head;
	count = env_lenght(env_list);
	converted_env_list = ft_malloc(count + 1, sizeof(char *));
	if (!converted_env_list)
		return (ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
	env_node = env_list->head;
	count = 0;
	while (env_node)
	{
		if (env_converter_export_loop(env_node, converted_env_list, &count))
			return (NULL);
		env_node = env_node->next;
	}
	converted_env_list[count] = NULL;
	return (converted_env_list);
}
