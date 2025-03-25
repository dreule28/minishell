#include "executor.h"

int	redir_infile(t_cmd_list *cmd_list, int *pipe_fd, int file_redirecting)
{
	t_file_node *file_node;

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
			if(dup2(file_redirecting, STDOUT_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2", 2), -1);
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				return (ft_putstr_fd("Error using dup2", 2), -1);
		}
		file_node = file_node->next;
	}
	return (0);
}

int	redir_here_doc(t_cmd_list *cmd_list, int *pipe_fd, int file_redirecting)
{
	t_file_node *file_node;
	(void)pipe_fd;
	int temp_fd;
	char *line;
	(void)file_redirecting;
	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == HEREDOC)
		{
			temp_fd = open(file_node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (temp_fd == -1)
				ft_putstr_fd("Operation not permitted\n", 2);
			else
			{
				while (1)
				{
					line = readline("> ");
					if (ft_strncmp(line, file_node->filename, ft_strlen(file_node->filename)) == 0)
					{
						free(line);
						break ;
					}
					ft_putstr_fd(line, temp_fd);
					ft_putstr_fd("\n", temp_fd);
					free(line);
				}
				close(temp_fd);
			}
		}
		file_node = file_node->next;
	}
	return 0;
}

int redir_outfile(t_cmd_list *cmd_list, int *pipe_fd, int file_redirecting)
{
	t_file_node *file_node;
	(void)pipe_fd;
	(void)file_redirecting;
	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == OUTFILE)
		{
			if(open(file_node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644) == -1)
				ft_putstr_fd("Operation not permitted\n", 2);
		}
		file_node = file_node->next;
	}
	return 0;
}

int redir_append(t_cmd_list *cmd_list, int *pipe_fd, int file_redirecting)
{
	t_file_node *file_node;
	(void)pipe_fd;
	(void)file_redirecting;
	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == OUTFILE_APPEND)
		{
			if(open(file_node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644) == -1)
				ft_putstr_fd("Operation not permitted\n", 2);
		}
		file_node = file_node->next;
	}
	return 0;
}
