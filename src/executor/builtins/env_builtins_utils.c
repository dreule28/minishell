/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:11 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 18:24:47 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_first_char(char *str)
{
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
	{
		ft_putstr_fd("not a valid identifier\n", 2);
		return (-1);
	}
	return (0);
}

int	check_type_name(char *str)
{
	int	count;

	if (check_first_char(str) == -1)
		return (-1);
	count = 1;
	while (str[count] != '\0' && str[count] != '=')
	{
		if (str[count] == '+')
		{
			if (str[count + 1] == '=' && count > 0)
				break ;
			else
			{
				ft_putstr_fd("not a valid identifier\n", 2);
				return (-1);
			}
		}
		if (!(ft_isalnum(str[count]) || str[count] == '_'))
		{
			ft_putstr_fd("not a valid identifier\n", 2);
			return (-1);
		}
		count++;
	}
	return (0);
}

void	check_export_behavior(t_cmd_node *cmd_node, t_env_list *env_list,
		int count)
{
	char	*type;
	char	*value;

	if (add_or_attach(cmd_node->cmd[count]) == 0)
	{
		if (check_duplicates(cmd_node->cmd[count], env_list) == 0)
		{
			type = get_type(cmd_node->cmd[count]);
			value = get_value(cmd_node->cmd[count]);
			add_env_node(env_list, type, value);
		}
	}
	else
	{
		if (export_attach(cmd_node->cmd[count], env_list) == 0)
		{
			type = get_type(cmd_node->cmd[count]);
			value = get_value(cmd_node->cmd[count]);
			add_env_node(env_list, type, value);
		}
	}
}

void	export_variable(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int	count;
	int	not_valid;

	count = 1;
	not_valid = 0;
	while (cmd_node->cmd[count] != NULL)
	{
		if (check_type_name(cmd_node->cmd[count]) == 0)
			check_export_behavior(cmd_node, env_list, count);
		else
			not_valid = 1;
		count++;
	}
	*exit_code() = not_valid;
}

void	print_export_variables(t_env_list *env_list)
{
	int		count;
	char	**converted_env_list;

	count = 0;
	converted_env_list = env_converter_export(env_list);
	sort_env_array(converted_env_list);
	while (converted_env_list[count] != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(converted_env_list[count], 1);
		ft_putstr_fd("\n", 1);
		count++;
	}
}
