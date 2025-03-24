#include "executor.h"

void	access_infile(t_cmd_list *cmd_list)
{
	t_file_node *file_node;

	file_node = cmd_list->head->files->head;
	while (file_node != NULL)  
	{
		if (file_node->redir_type == INFILE)
		{
			if(access(file_node->filename, F_OK) == -1)
				ft_putstr_fd("No such file or directory\n", 2);
			else if(open(file_node->filename, O_RDONLY) == -1)
				ft_putstr_fd("Operation not permitted\n", 2);
		}
		file_node = file_node->next;
	}
}

void create_outfile(t_cmd_list *cmd_list)
{
	t_file_node *file_node;

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
}

void append_file(t_cmd_list *cmd_list)
{
	t_file_node *file_node;

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
}