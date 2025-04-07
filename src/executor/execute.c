
#include "minishell.h"

void multiple_cmd_execution(t_cmd_list *cmd_list, char **env)
{
	(void)cmd_list;
	(void)env;
	// multiple command execution
}

void execute(char **env)
{
	//// testing functions
	t_cmd_list *cmd_list;   // declare a pointer to a struct of type t_cmd_list
	init_structs(&cmd_list); // initialize the struct
	fill_structs(cmd_list);	// fill the struct with data
	// print_structs(cmd_list); // print the struct
	////main functions

	if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
		single_builtin_execution(cmd_list); // single builtin
	else if (cmd_list->size == 1 && cmd_list->head->cmd_type == CMD)
		single_cmd_execution(cmd_list, env); // singel command
	else
		multiple_cmd_execution(cmd_list, env); // multiple commands
}

void	single_builtin_execution(t_cmd_list *cmd_list)
{
	if(strcmp(cmd_list->head->cmd[0], "exit") == 0)
		exit(0);
}

void single_cmd_execution(t_cmd_list *cmd_list, char **env)
{
	int pipe_fd[2];
	int saved_stdin = 0;
	int saved_stdout = 0;
	t_cmd_node *node;
	pid_t	pid;
	
	node = cmd_list->head;
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	while(node)	
	{
		if(node->next)
			fd_error_handle(pipe_fd);
		else
		{	pipe_fd[0] = saved_stdin;
			pipe_fd[1] = saved_stdout;
		}
		pid = fork();
		if(pid < 0)
			return (ft_putstr_fd("Error while forking", 2));
		if(pid == 0)
			child_proccess(cmd_list, pipe_fd, env);
		else
		{
			waitpid(pid, NULL, 0);
			parent_proccess(cmd_list, pipe_fd);
		}
		node = node->next;
	}
	reset_redirection(saved_stdin, saved_stdout);
}

char *env_search_path(void)
{
	// t_env_list *env_path;

	// while(env_path->head->next != NULL)
	// {
	// 	if(ft_strncmp(env_path->head->type, "PATH", 4) == 0)
	// 		return(env_path->head->value);
	// 	env_path->head = env_path->head->next;
	// }
	ft_putstr_fd("PATH variable not found", 2);
	return(NULL);
}

char *create_command_path(t_cmd_list *cmd_list, char **env_path_list)
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
		full_cmd_path = ft_strjoin(temp_path, cmd_list->head->cmd[0]);
		free(temp_path);
		if(access(full_cmd_path, F_OK) == 0)
		{
			free(temp_path);
			gc_add(full_cmd_path);
			return (full_cmd_path);
		}
		command_index++;
		free(full_cmd_path);
	}
	return(NULL);
}

char **env_converter(void)
{
	// convert the env list to a char ** for execve
	return(NULL);
}

void	execute_command(t_cmd_list *cmd_list, char **env_path_list, char **env)
{
	(void)env;
	char *full_cmd_path;
	char **converted_env_list;

	full_cmd_path = create_command_path(cmd_list, env_path_list);
	if (full_cmd_path == NULL)
	{
		ft_putstr_fd("Error: command not found\n", 2);
		return ;
	}
	converted_env_list = env_converter();
	execve(full_cmd_path, cmd_list->head->cmd, converted_env_list);
	ft_putstr_fd("Error: execve failed\n", 2);
}

void	child_proccess(t_cmd_list *cmd_list, int *pipe_fd, char **env)
{
	char *env_path_position;
	char **env_path_list;

	get_envs(env);
	if(file_redirecting_child(cmd_list, pipe_fd) == -1)
		return ; 
	env_path_position = env_search_path();
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

	// testing
	// int count;
	// while(env_path_list[count] != NULL)
	// {
	// 	printf("%s\n", env_path_list[count]);
	// 	count++;
	// }
	//
	// execute_command(cmd_list, env_path_list, env);
}

void	parent_proccess(t_cmd_list *cmd_list, int *pipe_fd)
{
	if(file_redirecting_parent(cmd_list, pipe_fd) == -1)
		return ; 

}






