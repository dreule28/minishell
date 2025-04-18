#include "minishell.h"

void	child_proccess(t_cmd_node *cmd_node, t_env_list *env_list)
{
	char	*env_path_position;
	char	**env_path_list;

	if (cmd_node->files->size > 0)
		if (file_redirecting(cmd_node, env_list) == -1)
			return ;
	env_path_position = env_search_path_var(env_list);
	// if (error_check_null(env_path_position) == -1)
	// 	return ;
	env_path_list = gc_split(env_path_position, ':');
	// if (error_check_null(env_path_list[0]) == -1)
	// 	return ;
	execute_command(cmd_node, env_path_list, env_list);
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
		if (access(full_cmd_path, F_OK) == 0)
		{
			gc_add(full_cmd_path);
			return (full_cmd_path);
		}
		command_index++;
		free(full_cmd_path);
	}
	return (NULL);
}

void	execute_command(t_cmd_node *cmd_node, char **env_path_list,
		t_env_list *env_list)
{
	char	*full_cmd_path;
	char	**converted_env_list;

	full_cmd_path = create_command_path(cmd_node, env_path_list);
	// if (error_check_null(full_cmd_path) == -1)
	// 	return ;
	converted_env_list = env_converter(env_list);
	// if (error_check_null(converted_env_list[0]) == -1)
	// 	return ;
	execve(full_cmd_path, cmd_node->cmd, converted_env_list);
	ft_putstr_fd("Error: execve failed\n", 2);
}
