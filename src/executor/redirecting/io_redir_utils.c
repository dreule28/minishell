#include "minishell.h"

int file_redirecting_child(t_cmd_node *cmd_node, int *pipe_fd)
{
	int outfile_check;

	outfile_check = 0;
	outfile_check = redir_loop(cmd_node, pipe_fd);
	if(outfile_check == -1)
		return(-1);
	return(0);
}

int file_redirecting_parent(t_cmd_node *cmd_node, int *pipe_fd)
{
	int outfile_check;
	int	saved_stdout;

	outfile_check = 0;
	if(cmd_node == NULL)
		return (OUTFILE_NOT_USED);
	saved_stdout = dup(STDOUT_FILENO);
	outfile_check = redir_loop(cmd_node, pipe_fd);
	if(outfile_check == -1)
		return(-1);
	else if(outfile_check == OUTFILE_NOT_USED)
	{
		if (dup2(saved_stdout, STDOUT_FILENO) == -1)
			return (ft_putstr_fd("Error using dup2(outfile)", 2), -1);
	}
	return(0);
}

int	redir_loop(t_cmd_node *cmd_node,int *pipe_fd)
{
	t_file_node *file_node;
	int outfile_status;
	int return_value;

	return_value = 0;
	outfile_status = OUTFILE_NOT_USED;
	if(cmd_node == NULL)
		return (OUTFILE_NOT_USED);
	file_node = cmd_node->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == INFILE)
			return_value = redir_infile(file_node, pipe_fd);
		else if (file_node->redir_type == HEREDOC)
			return_value = redir_here_doc(file_node, pipe_fd);
		else if (file_node->redir_type == OUTFILE)
			return_value = redir_outfile(file_node, pipe_fd);
		else if (file_node->redir_type == OUTFILE_APPEND)
			return_value = redir_append(file_node, pipe_fd);
		if(return_value == -1)
			return(-1);
		if (return_value != OUTFILE_USED)
			outfile_status = return_value;
		file_node = file_node->next;
	}
	return (outfile_status);
}
