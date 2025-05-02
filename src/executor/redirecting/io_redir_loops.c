#include "minishell.h"

int	create_here_doc_files(t_cmd_node *cmd_node, t_env_list *env_list,int count_here_doc)
{
	char	*file_name;
	t_file_node *file_node;

	file_node = cmd_node->files->head;
	while (file_node != NULL)
	{
		if(file_node->redir_type == TK_HEREDOC)
		{
			file_name = create_here_doc(file_node, env_list,count_here_doc);
			count_here_doc++;
			if (file_name == NULL)
				return (-1);
		}
		file_node = file_node->next;
	}
	return(count_here_doc);
}

int	redir_loop_infiles(t_cmd_node *cmd_node, t_env_list *env_list)
{
	t_file_node	*file_node;
	int			infile_status;
	int			return_value;
	static int	count_here_doc;

	return_value = 0;
	count_here_doc = 1;
	infile_status = INFILE_NOT_USED;
	if (cmd_node == NULL)
		return (INFILE_NOT_USED);
	file_node = cmd_node->files->head;
	while (file_node != NULL)
	{
		if (file_node->redir_type == TK_HEREDOC)
		{
			return_value = redir_here_doc(file_node, env_list, count_here_doc);
			count_here_doc++;
		}
		if (file_node->redir_type == TK_INFILE)
			return_value = redir_infile(file_node);
		if (return_value == -1)
			return (-1);
		if (return_value == INFILE_USED)
			infile_status = return_value;
		file_node = file_node->next;
	}
	return (infile_status);
}

int	redir_loop_outfiles(t_cmd_node *cmd_node)
{
	t_file_node	*file_node;
	int			outfile_status;
	int			return_value;

	return_value = 0;
	outfile_status = OUTFILE_NOT_USED;
	if (cmd_node == NULL)
		return (OUTFILE_NOT_USED);
	file_node = cmd_node->files->head;
	while (file_node != NULL)
	{
		if (file_node->redir_type == TK_OUTFILE)
			return_value = redir_outfile(file_node);
		else if (file_node->redir_type == TK_APPEND)
			return_value = redir_append(file_node);
		if (return_value == -1)
			return (-1);
		if (return_value == OUTFILE_USED)
			outfile_status = return_value;
		file_node = file_node->next;
	}
	return (outfile_status);
}
