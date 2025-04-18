#include "executor.h"
#include "minishell.h"

void execute(t_env_list *env_list, t_cmd_list *cmd_list)
{
	// DEBUG_INFO("starting execution\n");

	// if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
	// 	single_builtin_execution(cmd_list); // single builtin
	// else
	if (!cmd_list->syntax_error)
		execution(cmd_list, env_list);
	return ;
}


void	execution(t_cmd_list *cmd_list, t_env_list *env_list)
{
	int saved_stdin;
	int saved_stdout;

	saved_stdout = 0;
	saved_stdin = 0;
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	execution_loop(cmd_list, env_list);
	reset_redirection(saved_stdin, saved_stdout);
	close(saved_stdin);
	close(saved_stdout);
}


void execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
{
	pid_t pid;
	t_cmd_node *cmd_node;
	int pipe_fd[2];
	int prev_pipe_fd[2];
	int waited;

	prev_pipe_fd[0] = -1;
	prev_pipe_fd[1] = -1;
	pid = 0;
	cmd_node = cmd_list->head;
	while(cmd_node)
	{
		if(cmd_node->next)
			pipe_creation(pipe_fd);
		pid =  fork();
		if(pid < 0)	
			ft_putstr_fd("Error  forking\n", 2);	
		if(pid == 0)
		{
			if(set_pipes_child(cmd_node, pipe_fd, prev_pipe_fd) == -1)
				return ;
			// if(cmd_node->cmd_type == BUILTIN)
			// {
			// 	// single_builtin_execution(cmd_node, env_list);
			// 	exit(0);
			// }
			child_proccess(cmd_node, env_list);
			exit(0);
		}
		set_pipes_parent(pipe_fd, prev_pipe_fd);
		cmd_node = cmd_node->next;
	}
	while((waited = waitpid(-1, NULL, 0)) > 0)
		;
}
