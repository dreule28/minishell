/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:52:01 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/05 17:35:30 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int	search_absolut_path(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int			exit_status;
	struct stat	st;
	char		*single_cmd[2];

	exit_status = 0;
	if (access(cmd_node->cmd[0], F_OK | X_OK) == 0)
	{
		if (stat(cmd_node->cmd[0], &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("Error: is a directory\n", 2);
			return (126);
		}
		single_cmd[0] = cmd_node->cmd[0];
		single_cmd[1] = NULL;
		exit_status = execute_command(cmd_node, single_cmd, env_list);
	}
	else
	{
		ft_putstr_fd("Error: Command not found\n", 2);
		return (127);
	}
	return (exit_status);
}

int	child_proccess(t_cmd_node *cmd_node, t_env_list *env_list)
{
	char	*env_path_position;
	char	**env_path_list;
	int		exit_status;

	if (cmd_node->files->size > 0)
		if (file_redirecting(cmd_node) == -1)
			return (1);
	if (cmd_node->cmd == NULL || cmd_node->cmd[0] == NULL
		|| cmd_node->cmd[0][0] == '\0')
		return (0);
	if (ft_strchr(cmd_node->cmd[0], '/'))
		return (search_absolut_path(cmd_node, env_list));
	else
	{
		env_path_position = env_search_path_var(env_list);
		if (error_check_null(env_path_position) == -1)
			return (127);
		env_path_list = gc_split(env_path_position, ':');
		if (error_check_null(env_path_list[0]) == -1)
			return (127);
		exit_status = execute_command(cmd_node, env_path_list, env_list);
	}
	return (exit_status);
}

char	*access_full_path(t_cmd_node *cmd_node)
{
	struct stat	st;

	if (stat(cmd_node->cmd[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("Error: is a directory\n", 2);
		return (NULL);
	}
	return (cmd_node->cmd[0]);
}

char	*create_command_path(t_cmd_node *cmd_node, char **env_path_list)
{
	char	*temp_path;
	char	*full_cmd_path;
	int		command_index;

	command_index = 0;
	while (env_path_list[command_index] != NULL)
	{
		temp_path = ft_strjoin(env_path_list[command_index], "/");
		if (!temp_path)
			return (ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
		full_cmd_path = ft_strjoin(temp_path, cmd_node->cmd[0]);
		free(temp_path);
		if (access(full_cmd_path, F_OK | X_OK) == 0)
		{
			gc_add(full_cmd_path);
			return (full_cmd_path);
		}
		else if (access(cmd_node->cmd[0], F_OK | X_OK) == 0)
			return (access_full_path(cmd_node));
		command_index++;
		free(full_cmd_path);
	}
	print_error_message(cmd_node->cmd[0], "command not found");
	return (NULL);
}

int	execute_command(t_cmd_node *cmd_node, char **env_path_list,
		t_env_list *env_list)
{
	char	*full_cmd_path;
	char	**converted_env_list;

	full_cmd_path = create_command_path(cmd_node, env_path_list);
	if (full_cmd_path == NULL)
		return (127);
	converted_env_list = env_converter(env_list);
	if (error_check_null(converted_env_list[0]) == -1)
		return (127);
	execve(full_cmd_path, cmd_node->cmd, converted_env_list);
	ft_putstr_fd("Error: execve failed\n", 2);
	return (127);
}
