/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:04 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/06 10:40:06 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_env_array(char **converted_env_list)
{
	int		current;
	int		compare;
	char	*tmp;

	current = 0;
	while (converted_env_list[current] != NULL)
	{
		compare = current + 1;
		while (converted_env_list[compare] != NULL)
		{
			if (ft_strcmp(converted_env_list[current],
					converted_env_list[compare]) > 0)
			{
				tmp = converted_env_list[current];
				converted_env_list[current] = converted_env_list[compare];
				converted_env_list[compare] = tmp;
			}
			compare++;
		}
		current++;
	}
}

bool	is_all_flag(char *flag, char c)
{
	int	i;

	if (!flag || flag[0] != '-')
		return (false);
	i = 1;
	if (!flag[i])
		return (false);
	while (flag[i])
	{
		if (flag[i] != c)
			return (false);
		i++;
	}
	return (true);
}

int	ft_isnum(char *str)
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
		{
			ft_putstr_fd("numeric argument required\n", 2);
			return (-1);
		}
		has_digit = 1;
		count++;
	}
	if (!has_digit)
	{
		ft_putstr_fd("numeric argument required\n", 2);
		return (-1);
	}
	return (0);
}

int	check_exit_arguments(t_cmd_node *cmd_node)
{
	if (ft_isnum(cmd_node->cmd[1]) == -1)
	{
		*exit_code() = 255;
		return (-1);
	}
	if (cmd_node->cmd[1][0] == '-')
	{
		*exit_code() = 255;
		return (-1);
	}
	if (cmd_node->cmd[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		*exit_code() = 1;
		return (1);
	}
	return (0);
}
