#include "minishell.h"

int file_redirecting_child(t_cmd_list *cmd_list, int *pipe_fd)
{
	if(redir_loop(cmd_list, pipe_fd) == -1)
		return (-1);
	return(0);
}

int file_redirecting_parent(t_cmd_list *cmd_list, int *pipe_fd)
{
	if(redir_loop(cmd_list, pipe_fd) == -1)
		return (-1);
	return(0);
}

int	redir_loop(t_cmd_list *cmd_list,int *pipe_fd)
{
	t_file_node *file_node;
	int error_check;

	error_check = 0;
	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == INFILE)
			error_check = redir_infile(file_node, pipe_fd);
		if (file_node->redir_type == HEREDOC)
			error_check = redir_here_doc(file_node, pipe_fd);
		if (file_node->redir_type == OUTFILE)
			error_check = redir_outfile(file_node, pipe_fd);
		if (file_node->redir_type == OUTFILE_APPEND)
			error_check = redir_append(file_node, pipe_fd);
		file_node = file_node->next;
	}
	cmd_list->head = cmd_list->head->next;
	return (error_check);
}
