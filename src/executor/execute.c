#include "minishell.h"

void execute(char **env)
{
	//// testing functions
	t_cmd_list *cmd_list;   // declare a pointer to a struct of type t_cmd_list
	init_structs(&cmd_list); // initialize the struct
	fill_structs(cmd_list);	// fill the struct with data
	// print_structs(cmd_list); // print the struct
	////main functions

	if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
		execute_builtin(cmd_list); // single builtin
	else
		child_parent_proccess(cmd_list, env); // singel command
}

void	execute_builtin(t_cmd_list *cmd_list)
{
	if(strcmp(cmd_list->head->cmd[0], "exit") == 0)
		exit(0);
}

void child_parent_proccess(t_cmd_list *cmd_list, char **env)
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
	t_env_list *env_path;

	while(env_path->head->next != NULL)
	{
		if(ft_strncmp(env_path->head->type, "PATH", 4) == 0)
			return(env_path->head->value);
		env_path->head = env_path->head->next;
	}
	ft_putstr_fd("PATH variable not found", 2);
	return(NULL);
}

void	child_proccess(t_cmd_list *cmd_list, int *pipe_fd, char **env)
{
	char *env_path_value;
	char **path;

	get_envs(env);
	if(file_redirecting_child(cmd_list, pipe_fd) == -1)
		return ; 
	env_path_value = env_search_path();
	if(env_path_value == NULL)
	{
		ft_putstr_fd("Error: env not found\n", 2);
		return ;
	}
	path = gc_split(env_path_value, ':');
	if(!path)
	{
		ft_putstr_fd("Error: path not found\n", 2);
		return ;
	}
	int count;
	while(path[count] != NULL)
	{
		printf("%s\n", path[count]);
		count++;
	}
}

void	parent_proccess(t_cmd_list *cmd_list, int *pipe_fd)
{
	(void)cmd_list;
	(void)pipe_fd;
	// if(file_redirecting_parent(cmd_list, pipe_fd) == -1)
	// 	return ; 
	// write(1, "hello, parent\n\0", 16);

}






