#include "minishell.h"

void	execute_builtin(t_cmd_list *cmd_list)
{
	(void)cmd_list;
}

void	fd_error_handle(int *fd)
{
	if (pipe(fd) == -1)
	{
		ft_putstr_fd("Error: invalid file", 2);
		return ;
	}
}

int file_redirecting_child(t_cmd_list *cmd_list, int *pipe_fd)
{
	int fd;

	fd = 0;
	fd = redir_infile(cmd_list, pipe_fd, fd);
	fd = redir_here_doc(cmd_list, pipe_fd, fd);
	if(fd <= 0)
		fd = pipe_fd[0];
	fd = redir_outfile(cmd_list, pipe_fd, fd);
	fd = redir_append(cmd_list, pipe_fd, fd);

	if(dup2(fd, STDIN_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2", 2), -1);

	if(fd != pipe_fd[0])
		close(fd);
	return(0);
}

int file_redirecting_parent(t_cmd_list *cmd_list, int *pipe_fd)
{
	int fd;

	fd = pipe_fd[1];
	fd = redir_infile(cmd_list, pipe_fd, fd);
	fd = redir_here_doc(cmd_list, pipe_fd, fd);
	fd = redir_outfile(cmd_list, pipe_fd, fd);
	fd = redir_append(cmd_list, pipe_fd, fd);

	if(dup2(fd, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2", 2), -1);

	if(fd != pipe_fd[1])
		close(fd);
	return(0);
}

void	child_proccess(t_cmd_list *cmd_list, int *pipe_fd, char **env)
{
	(void)env;

	if(file_redirecting_child(cmd_list, pipe_fd) == -1)
		return ; 
	write(1, "hello\n", 5);
}

void	parent_proccess(t_cmd_list *cmd_list, int *pipe_fd, char **env)
{
	(void)env;

	if(file_redirecting_parent(cmd_list, pipe_fd) == -1)
		return ; 

}

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

void child_parent_proccess(t_cmd_list *cmd_list, char **env)
{
	int pipe_fd[2];
	int count = 0;
	int saved_stdin = 0;
	int saved_stdout = 0;
	pid_t	pid;
	
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	while(cmd_list->size > count)	
	{
		fd_error_handle(pipe_fd);
		pid = fork();
		if(pid < 0)
			return (ft_putstr_fd("Error while forking", 2));
		if(pid == 0)
			child_proccess(cmd_list, pipe_fd, env);
		else
		{
			waitpid(pid, NULL, 0);
			parent_proccess(cmd_list, pipe_fd, env);
		}
		count++;
	}
	reset_redirection(saved_stdin, saved_stdout);
}

void	create_proccesses(t_cmd_list *cmd_list, char **env)
{
	if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
		execute_builtin(cmd_list);
	else if(cmd_list->head->cmd_type != PIPE)
		child_parent_proccess(cmd_list, env);
	
}

void execute(char **env)
{
	//// testing functions
    t_cmd_list *cmd_list;   // declare a pointer to a struct of type t_cmd_list
    init_structs(&cmd_list); // initialize the struct
    fill_structs(cmd_list);	// fill the struct with data
	print_structs(cmd_list); // print the struct
	////main functions
	create_proccesses(cmd_list, env);


	///testing functions


}

