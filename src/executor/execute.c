#include "minishell.h"

int execute(t_env_list *env_list, t_cmd_list *cmd_list)
{
	if (cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
	{
		if (single_builtin_execution(cmd_list->head, env_list) == -1)
			return (0);
	}
	else
		execution(cmd_list, env_list);
	return (1);
}


void	execution(t_cmd_list *cmd_list, t_env_list *env_list)
{
	int saved_stdin;
	int saved_stdout;

	saved_stdout = 0;
	saved_stdin = 0;
	execution_loop(cmd_list, env_list);
	save_stdin_stdout(&saved_stdin, &saved_stdout);
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
	int exit_status;

	prev_pipe_fd[0] = -1;
	prev_pipe_fd[1] = -1;
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	pid = 0;
	cmd_node = cmd_list->head;

	g_sigint_status = 3;
	while(cmd_node)
	{
		if(create_here_doc_files(cmd_node, env_list) == -1)
			return ;
		if(cmd_node->next)
			pipe_creation(pipe_fd);
		pid =  fork();
		if(pid < 0)
			return (ft_putstr_fd("\033Error  forking", 2));
		else if(pid == 0)
		{
			if(set_pipes_child(cmd_node, pipe_fd, prev_pipe_fd) == -1)
				return ;
			if(cmd_node->cmd_type == BUILTIN)
				exit_status = single_builtin_execution(cmd_node, env_list);
			else
				exit_status = child_proccess(cmd_node, env_list);
			clean_up();
			exit(exit_status);
		}
		set_pipes_parent(pipe_fd, prev_pipe_fd);
		cmd_node = cmd_node->next;
	}
	int status;
	while ((waited = waitpid(-1, &status, 0)) > 0)
		;
	g_sigint_status = 0;
	WIFEXITED(status);
	if (WIFSIGNALED(status))
		*exit_code() = 127 + WTERMSIG(status);
	else
		*exit_code() = WEXITSTATUS(status);
}
