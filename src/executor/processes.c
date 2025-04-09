#include "minishell.h"

void	child_proccess(t_cmd_node *cmd_node, int *pipe_fd, t_env_list *env_list)
{
	char *env_path_position;
	char **env_path_list;

	if(file_redirecting_child(cmd_node, pipe_fd) == -1)
		return ; 
	env_path_position = env_search_path_var(env_list);
	if(env_path_position == NULL)
	{
		ft_putstr_fd("Error: env not found\n", 2);
		return ;
	}
	env_path_list = gc_split(env_path_position, ':');
	if(!env_path_list)
	{
		ft_putstr_fd("Error: path not found\n", 2);
		return ;
	}
	
	execute_command(cmd_node, env_path_list, env_list);
}

void	parent_proccess(t_cmd_node *cmd_node, int *pipe_fd, t_env_list *env_list)
{	
	(void)env_list;
	// (void)pipe_fd;
	if(cmd_node == NULL)
		return ;
	if(file_redirecting_parent(cmd_node, pipe_fd) == -1)
		return ;
}

char *create_command_path(t_cmd_node *cmd_node, char **env_path_list)
{
	char *temp_path;
	char *full_cmd_path;
	int	command_index;

	command_index = 0;
	while(env_path_list[command_index] != NULL)
	{
		temp_path = ft_strjoin(env_path_list[command_index], "/");
		if(!temp_path)
			return(ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
		full_cmd_path = ft_strjoin(temp_path, cmd_node->cmd[0]);
		free(temp_path);
		if(access(full_cmd_path, F_OK) == 0)
		{
			gc_add(full_cmd_path);
			return (full_cmd_path);
		}
		command_index++;
		// printf("%s\n", full_cmd_path);
		free(full_cmd_path);
	}
	return(NULL);
}

void	execute_command(t_cmd_node *cmd_node, char **env_path_list, t_env_list *env_list)
{
	char *full_cmd_path;
	char **converted_env_list;

	full_cmd_path = create_command_path(cmd_node, env_path_list);
	if (full_cmd_path == NULL)
	{
		ft_putstr_fd("Error: command not found\n", 2);
		return ;
	}
	converted_env_list = env_converter(env_list);
	if(!converted_env_list)
		return ;
		
	// write(1, "hallo\n", 6);
	execve(full_cmd_path, cmd_node->cmd, converted_env_list);
	ft_putstr_fd("Error: execve failed\n", 2);
}