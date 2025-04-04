#include "minishell.h"

void reset_redirection(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
}

void save_stdin_stdout(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

void	fd_error_handle(int *fd)
{
	if (pipe(fd) == -1)
	{
		ft_putstr_fd("Error: invalid file", 2);
		return ;
	}
}