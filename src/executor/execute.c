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
	// t_file_node *file_node;
	cmd_list = init_cmd_list(); // initialize the struct
	
	// first command
	
	t_cmd_node *cmd_node1; // declare a pointer to a struct of type t_cmd_node1
	char *cmd[] = {"cat", NULL};
	cmd_node1 = add_cmd_node(cmd_list,cmd, CMD); // add a node to the struct
	// (void)cmd_node1;
	add_file_node(cmd_node1->files, "END", INFILE); // add a file node to the struct
	// add_file_node(cmd_node1->files, "outfile", OUTFILE);

	// second command
	
	// t_cmd_node *cmd_node2;
	// cmd_node2 = add_cmd_node(cmd_list, cmd, CMD); // add a node to the struct
	// add_file_node(cmd_node2->files, "END2", OUTFILE); // add a file node to the struct

	// thired command
	

	////main functions

	if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
		single_builtin_execution(cmd_list); // single builtin
	else
		single_cmd_execution(cmd_list, env); // singel command
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
			child_proccess(node, pipe_fd, env);
		else
		{
			waitpid(pid, NULL, 0);
			printf("pid: %d\n", pid);
			if(node->next)
				parent_proccess(node->next, pipe_fd);
		}
		node = node->next;
	}
	reset_redirection(saved_stdin, saved_stdout);
}

char *env_search_path(t_env_list *env_list)
{

	t_env_node *node;

	node = env_list->head;
	while(node->next != NULL)
	{
		if(ft_strncmp(node->type, "PATH", 4) == 0)
			return(node->value);
		node = node->next;
	}
	ft_putstr_fd("PATH variable not found", 2);
	return(NULL);
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
		free(full_cmd_path);
	}
	return(NULL);
}

int env_size(t_env_list *env_list)
{
	int count = 0;
	t_env_node *node;

	node = env_list->head;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

char **env_converter(t_env_list *env_list)
{
	int count = 0;
	t_env_node *env_node;
	char **converted_env_list;
	char *tmp, *env_str;

	count = 0;
	env_node = env_list->head;
	count = env_size(env_list);
	converted_env_list = ft_malloc(count + 1, sizeof(char *));
	if (!converted_env_list)
		return(ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
	env_node = env_list->head;
	count = 0;
	while (env_node)
	{
		tmp = ft_strjoin(env_node->type, "=");
		env_str = gc_strjoin(tmp, env_node->value);
		free(tmp);
		if (!env_str)
			return(ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
		converted_env_list[count++] = env_str;
		env_node = env_node->next;
	}
	converted_env_list[count] = NULL;
	return (converted_env_list);
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
	execve(full_cmd_path, cmd_node->cmd, converted_env_list);
	ft_putstr_fd("Error: execve failed\n", 2);
}

void	child_proccess(t_cmd_node *cmd_node, int *pipe_fd, char **env)
{
	t_env_list *env_list;
	char *env_path_position;
	char **env_path_list;
	
	env_list = get_envs(env);
	if(file_redirecting_child(cmd_node, pipe_fd) == -1)
		return ; 
	env_path_position = env_search_path(env_list);
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

void	parent_proccess(t_cmd_node *cmd_node, int *pipe_fd)
{
	if(file_redirecting_parent(cmd_node, pipe_fd) == -1)
		return ; 
}






