# include "executor.h"

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
