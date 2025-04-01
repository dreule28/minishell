#include "executor.h"

int	redir_infile(t_cmd_list *cmd_list, int *pipe_fd)
{
	t_file_node *file_node;
	int file_redirecting;

	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == INFILE)
		{
			if(access(file_node->filename, F_OK) == -1)
				ft_putstr_fd("No such file or directory\n", 2);
			else 
			{
				file_redirecting = open(file_node->filename, O_RDONLY);
				if(file_redirecting == -1)
					return (-1);
			}
			if(dup2(file_redirecting, STDIN_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2(infile)", 2), -1);
		}
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (ft_putstr_fd("Error using dup2(infile)", 2), -1);
		file_node = file_node->next;
	}
	return (0);
}

int	create_here_doc(t_file_node *file_node)
{
	int fd;
	char *line;

	fd = open(file_node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ft_putstr_fd("Operation not permitted\n", 2);
	while (1)
	{
		line = readline("> ");
		if(!line)
			break ;
		if (ft_strncmp(line, file_node->filename, ft_strlen(file_node->filename)) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	return(fd);
}

int	redir_here_doc(t_cmd_list *cmd_list, int *pipe_fd)
{
	t_file_node *file_node;
	int file_redirecting;
	int saved_stdin;
	
	file_redirecting = 0;
	file_node = cmd_list->head->files->head;
	saved_stdin = dup(STDIN_FILENO);
	while (file_node != NULL)  
	{
		if (file_node->redir_type == HEREDOC)
		{
			file_redirecting = create_here_doc(file_node);
			if(file_redirecting == -1)
				return (-1);
			if(dup2(file_redirecting, STDIN_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2(here_doc)", 2), -1);
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2(here_doc)", 2), -1);
			dup2(saved_stdin, STDIN_FILENO);
			close(file_redirecting);
		}
		file_node = file_node->next;
	}
	close(saved_stdin);
	return (0);
}

int redir_outfile(t_cmd_list *cmd_list, int *pipe_fd)
{
	t_file_node *file_node;
	int file_redirecting;

	(void)pipe_fd;
	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == OUTFILE)
		{
			file_redirecting = open(file_node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(file_redirecting == -1)
				ft_putstr_fd("Operation not permitted\n", 2);
			if(dup2(pipe_fd[0], STDIN_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2(outfile)stdin\n", 2), -1);
			if (dup2(file_redirecting, STDOUT_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2(outfile)stdout\n", 2), -1);
		}
		file_node = file_node->next;
	}
	return (0);
}

int redir_append(t_cmd_list *cmd_list, int *pipe_fd)
{
	t_file_node *file_node;
	int file_redirecting;

	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == OUTFILE_APPEND)
		{
			file_redirecting = open(file_node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if(file_redirecting == -1)
				ft_putstr_fd("Operation not permitted\n", 2);
			if(dup2(pipe_fd[0], STDIN_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2(append)", 2), -1);
			if (dup2(file_redirecting, STDOUT_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2(append)", 2), -1);
		}
		file_node = file_node->next;
	}
	return (0);
}
