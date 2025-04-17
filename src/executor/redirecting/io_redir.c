#include "executor.h"

int	redir_infile(t_file_node *file_node)
{
	int file_redirecting;

	if(access(file_node->filename, F_OK) == -1)
		ft_putstr_fd("\033No such file or directory\n", 2);
	file_redirecting = open(file_node->filename, O_RDONLY);
	if(file_redirecting == -1)
		return (-1);
	if(dup2(file_redirecting, STDIN_FILENO) == -1)
		return (ft_putstr_fd("\033Error using dup2(infile)stdin\n", 2), -1);
	close(file_redirecting);
	return (INFILE_USED);
}


int	redir_here_doc(t_file_node *file_node)
{
	int file_redirecting;
	char *file_name;

	file_redirecting = 0;
	file_name = create_here_doc(file_node);
	if(file_name == NULL)
		return(-1);
	file_redirecting = open(file_name, O_RDONLY);
    if (file_redirecting == -1)
    {
		ft_putstr_fd("Operation not permitted\n", 2);
		return(-1);
	}
	if(dup2(file_redirecting, STDIN_FILENO) == -1)
	{
		ft_putstr_fd("\033Error using dup2(here_doc)stdin\n", 2);
		return (-1);
	}
	close(file_redirecting);
	return (INFILE_USED);
}

char	*create_here_doc(t_file_node *file_node)
{
	int write_fd;
	char *line;
	char *str;

	set_interaktive_line();
	
	str = gc_strjoin("tmp/.", file_node->filename);
	write_fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (write_fd == -1)
		return(ft_putstr_fd("Operation not permitted\n", 2), NULL);
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
		ft_putstr_fd(line, write_fd);
		ft_putstr_fd("\n", write_fd);
		free(line);
	}
	close(write_fd);
    return(str);
}

int redir_outfile(t_file_node *file_node)
{
	int file_redirecting;
	file_redirecting = open(file_node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(file_redirecting == -1)
		ft_putstr_fd("Operation not permitted\n", 2);
	if (dup2(file_redirecting, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2(outfile)stdout\n", 2), -1);
	close(file_redirecting);
	return (OUTFILE_USED);
}

int redir_append(t_file_node *file_node)
{
	int file_redirecting;
	file_redirecting = open(file_node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(file_redirecting == -1)
		ft_putstr_fd("Operation not permitted\n", 2);
	if (dup2(file_redirecting, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2(append)stdout\n", 2), -1);
	close(file_redirecting);
	return (OUTFILE_USED);
}
