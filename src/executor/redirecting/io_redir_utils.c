#include "minishell.h"

int file_redirecting_child(t_cmd_node *cmd_node, int *pipe_fd, t_cmd_list *cmd_list)
{
    int file_check_outfile;
	int file_check_infile;
    
		(void)cmd_list;
	file_check_infile = redir_loop_infiles(cmd_node, pipe_fd);
	// DEBUG_INFO("file_redir_child enum : %d", file_check_infile);
	// DEBUG_INFO("HEAD ADRESS: %p \t NODE ADRESS: %p", cmd_list->head, cmd_node);
    // if (file_check_infile == INFILE_NOT_USED && cmd_list->head != cmd_node)
    // {
		//     if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
    //         return (ft_putstr_fd("Error using dup2(infile)", 2), -1);
	// }
	file_check_outfile = redir_loop_outfiles(cmd_node, pipe_fd);
    if (file_check_outfile == -1)
		return (-1);
	if (file_check_infile == -1)
		return (-1);
    // if (file_check_outfile == OUTFILE_NOT_USED && cmd_node->next != NULL )
    // {
    //     if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
    //         return (ft_putstr_fd("Error using dup2(outfile)", 2), -1);
    // }
	return (0);
}


int redir_loop_infiles(t_cmd_node *cmd_node, int *pipe_fd)
{
	t_file_node *file_node;
	int infile_status;
	int return_value;

	return_value = 0;
	infile_status = INFILE_NOT_USED;
	if(cmd_node == NULL)
		return (INFILE_NOT_USED);
	file_node = cmd_node->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == INFILE)
		{
			return_value = redir_infile(file_node, pipe_fd);
			// DEBUG_INFO("ENUM: %d", return_value);
		}
		else if (file_node->redir_type == HEREDOC)
		{
			return_value = redir_here_doc(file_node, pipe_fd);
			// DEBUG_INFO("ENUM: %d", return_value);
		}
		if(return_value == -1)
			return(-1);
		if (return_value == INFILE_USED)
		{
			infile_status = return_value;
			// DEBUG_INFO("STATUS: %d", infile_status);
		}
		file_node = file_node->next;
	}
	return (infile_status);
}

int	redir_loop_outfiles(t_cmd_node *cmd_node,int *pipe_fd)
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
		if (file_node->redir_type == OUTFILE)
			return_value = redir_outfile(file_node, pipe_fd);
		else if (file_node->redir_type == OUTFILE_APPEND)
			return_value = redir_append(file_node, pipe_fd);
		if(return_value == -1)
			return(-1);
		if (return_value == OUTFILE_USED)
			outfile_status = return_value;
		file_node = file_node->next;
	}
	return (outfile_status);
}
