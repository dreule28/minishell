#include "minishell.h"

void	close_pipes(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	set_invalid_pipe(int *prev_pipe_fd)
{
	prev_pipe_fd[0] = -1;
	prev_pipe_fd[1] = -1;
}

void	set_new_prev_pipe(int *prev_pipe_fd, int *pipe_fd)
{
	prev_pipe_fd[0] = pipe_fd[0];
	prev_pipe_fd[1] = pipe_fd[1];
}

int	set_pipes_child(t_cmd_node *cmd_node, int *pipe_fd, int *prev_pipe_fd)
	{
		if (prev_pipe_fd[0] != -1)
			if (dup2(prev_pipe_fd[0], STDIN_FILENO) == -1)
				return (-1);
		if (cmd_node->next)
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				return (-1);
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		if (prev_pipe_fd[0] != -1)
			close(prev_pipe_fd[0]);
		if (prev_pipe_fd[1] != -1)
			close(prev_pipe_fd[1]);
		return (0);
}

void	set_pipes_parent(int *pipe_fd, int *prev_pipe_fd)
{
	if (prev_pipe_fd[0] != -1)
		close(prev_pipe_fd[0]);
	if (prev_pipe_fd[1] != -1)
		close(prev_pipe_fd[1]);
	prev_pipe_fd[0] = pipe_fd[0];
	prev_pipe_fd[1] = pipe_fd[1];
}