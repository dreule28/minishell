#include "executor.h"

void testing_function(t_cmd_list *cmd_list)
{
	//// testing functions
	// first command
	

	// add_file_node(cmd_node1->files, "outfile", OUTFILE);

	// second command
	(void)cmd_list;
	// t_cmd_node *cmd_node2;
	// cmd_node2 = add_cmd_node(cmd_list, cmd, CMD); // add a node to the struct
	// add_file_node(cmd_node2->files, "END2", OUTFILE); // add a file node to the struct

	// thired command
	
}

void execute(char **env)
{
	t_env_list *env_list;
	
	
	t_cmd_list *cmd_list;
	cmd_list = init_cmd_list();

	t_cmd_node *cmd_node1; // declare a pointer to a struct of type t_cmd_node1
	char *cmd1[] = {"cat", NULL};
	cmd_node1 = add_cmd_node(cmd_list,cmd1, CMD); // add a node to the struct
	add_file_node(cmd_node1->files, "END", INFILE);

	t_cmd_node *cmd_node2; // declare a pointer to a struct of type t_cmd_node1
	char *cmd2[] = {"cat", NULL};
	cmd_node2 = add_cmd_node(cmd_list,cmd2, CMD); // add a node to the struct
	add_file_node(cmd_node2->files, "hallöchen", OUTFILE_APPEND);

	t_cmd_node *cmd_node3; // declare a pointer to a struct of type t_cmd_node1
	char *cmd3[] = {"ls", "-al", NULL};
	cmd_node3 = add_cmd_node(cmd_list,cmd3, CMD); // add a node to the struct
	add_file_node(cmd_node3->files, "pups", OUTFILE_APPEND);

	env_list = get_envs(env);

		printf("%s\n" , cmd_list->head->cmd[0]);
	if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
		single_builtin_execution(cmd_list); // single builtin
	else
		execution(cmd_list, env_list); // normal execution
}

void	single_builtin_execution(t_cmd_list *cmd_list)
{
	if(strcmp(cmd_list->head->cmd[0], "exit") == 0)
		exit(0);
}
/*
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
			pipe_creation(pipe_fd);
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
			if(node->next)
				parent_proccess(node->next, pipe_fd);
		}
		node = node->next;
	}
	reset_redirection(saved_stdin, saved_stdout);
}

*/

void	execution(t_cmd_list *cmd_list, t_env_list *env_list)
{
	int saved_stdin;
	int saved_stdout;

	saved_stdout = 0;
	saved_stdin = 0;
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	execution_loop(cmd_list, env_list);
	reset_redirection(saved_stdin, saved_stdout);
}

void execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
{
	pid_t pid;
	int pipe_fd[2];
	int prev_pipe_fd[2];
	t_cmd_node	*cmd_node;

	cmd_node = cmd_list->head;
	while(cmd_node)
	{
		if(cmd_node->next)
			pipe_creation(pipe_fd);
		else
			dup_stdin_stdout(pipe_fd);
		pid = fork();
		if(pid < 0)
			return (ft_putstr_fd("Error while forking", 2));
		if(pid == 0)
		{
			if(prev_pipe_fd[0] == -1)
				dup2(prev_pipe_fd[0], STDIN_FILENO);
			if(cmd_node->next)
				dup2(pipe_fd[1], STDOUT_FILENO);
			if(prev_pipe_fd[0] == -1)
				close_pipes(prev_pipe_fd);
			if(cmd_node->next)
				close_pipes(pipe_fd);
			child_proccess(cmd_node, pipe_fd, env_list);
			}
			else
			{
			waitpid(pid, NULL, 0);
			if(prev_pipe_fd[0] == -1)
				close_pipes(prev_pipe_fd);
			if(cmd_node->next)
			{	
				set_new_prev_pipe(pipe_fd, prev_pipe_fd);
				parent_proccess(cmd_node, prev_pipe_fd, env_list);
			}
		}
		cmd_node = cmd_node->next;
	}
	if(prev_pipe_fd[0] == -1)
		close_pipes(prev_pipe_fd);
}

void	dup_stdin_stdout(int *pipe_fd)
{
	pipe_fd[0] = dup(STDIN_FILENO);
	pipe_fd[1] = dup(STDOUT_FILENO);
}
void	close_pipes(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	set_invalid_pipe(int *pipe_fd)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
}

void	set_new_prev_pipe(int *pipe_fd, int *prev_pipe_fd)
{
	prev_pipe_fd[0] = pipe_fd[0];
	prev_pipe_fd[1] = pipe_fd[1];
}










