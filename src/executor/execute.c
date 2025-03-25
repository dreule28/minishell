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

int child_redirecting(t_cmd_list *cmd_list, int *pipe_fd)
{
	int valid;
	int file_redirecting;

	file_redirecting = 0;
	valid = redir_infile(cmd_list, pipe_fd, file_redirecting);
	valid = redir_here_doc(cmd_list, pipe_fd, file_redirecting);
	valid = redir_outfile(cmd_list, pipe_fd, file_redirecting);
	valid = redir_append(cmd_list, pipe_fd, file_redirecting);
	
	close(pipe_fd[1]);
	close(file_redirecting);
	return(valid);
}

void	child_proccess(t_cmd_list *cmd_list, int *pipe_fd, char **env)
{
	(void)cmd_list;
	(void)env;
	printf("hallo child \n");

	if(child_redirecting(cmd_list, pipe_fd) == -1)
		return ;
	
}

void	parent_proccess(t_cmd_list *cmd_list, int *pipe_fd, char **env)
{
	(void)cmd_list;
	(void)env;
	(void)pipe_fd;
	printf("hallo prents\n");
}

void child_parent_proccess(t_cmd_list *cmd_list, char **env)
{
	int pipe_fd[2];
	int count;
	pid_t	pid;

	while(cmd_list->size > count)	
	{
		fd_error_handle(pipe_fd);
		pid = fork();
		if(pid < 0)
			return (ft_putstr_fd("Error while forking", 2));
		if(pid == 0)
		{
			close(pipe_fd[0]);
			child_proccess(cmd_list, pipe_fd, env);
		}
		else
		{
			close(pipe_fd[1]);
			parent_proccess(cmd_list, pipe_fd, env);
			waitpid(pid, NULL, 0);
		}
		printf("piped\n");
		count++;
	}
}

void	create_proccesses(t_cmd_list *cmd_list, char **env)
{
	if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
		execute_builtin(cmd_list);
	else
		child_parent_proccess(cmd_list, env);
}

void execute(char **env)
{
	//// testing functions
    t_cmd_list *cmd_list;   // declare a pointer to a struct of type t_cmd_list
    init_structs(&cmd_list); // initialize the struct
    fill_structs(cmd_list);	// fill the struct with data
	print_structs(cmd_list); // print the struct
	////
	create_proccesses(cmd_list, env);

}

