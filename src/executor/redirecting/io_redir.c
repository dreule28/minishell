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


int	redir_here_doc(t_file_node *file_node, t_env_list *env_list)
{
	int file_redirecting;
	int saved_stdin;
	int saved_stdout;


	file_redirecting = 0;
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	file_redirecting = create_here_doc(file_node, env_list);
	if(file_redirecting == -1)
		return (-1);
	if(dup2(file_redirecting, STDIN_FILENO) == -1)
		return (ft_putstr_fd("\033Error using dup2(here_doc)stdin\n", 2), -1);
	close(file_redirecting);
	reset_redirection(saved_stdin, saved_stdout);
	close(saved_stdin);
	close(saved_stdout);
	return (INFILE_USED);
}

int	create_here_doc(t_file_node *file_node, t_env_list *env_list)
{
	int fd;
	char *line;
	char *str;
	t_token	*token;

	str = gc_strjoin("tmp/.", file_node->filename);
	DEBUG_INFO("line in heredoc: %s", str);
	fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return(ft_putstr_fd("Operation not permitted\n", 2), -1);
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
		token = ft_malloc(sizeof(t_token), 1);
		token->value = gc_strdup(line);
		token->segment_list = NULL;
		split_token_into_heredocs(token);
		expand_token(token, env_list);
		ft_putstr_fd(token->value, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	return(fd);
}

int redir_outfile(t_file_node *file_node)
{
	int file_redirecting;
	file_redirecting = open(file_node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(file_redirecting == -1)
		ft_putstr_fd("Operation not permitted\n", 2);
	// if(dup2(pipe_fd[0], STDIN_FILENO) == -1)
	// 	return (ft_putstr_fd("Error using dup2(outfile)stdin\n", 2), -1);
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
	// if(dup2(pipe_fd[0], STDIN_FILENO) == -1)
	// 	return (ft_putstr_fd("Error using dup2(append)stdin\n", 2), -1);
	if (dup2(file_redirecting, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2(append)stdout\n", 2), -1);
	close(file_redirecting);
	return (OUTFILE_USED);
}
