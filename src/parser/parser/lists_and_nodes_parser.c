#include "minishell.h"

t_cmd_list	*init_cmd_list(void)
{
	t_cmd_list	*cmd_list;

	cmd_list = ft_malloc(sizeof(t_cmd_list), 1);
	if (!cmd_list)
		return (NULL);
	cmd_list->head = NULL;
	cmd_list->tail = NULL;
	cmd_list->tail = 0;
	return (cmd_list);
}

void	add_cmd_node(t_cmd_list *cmd_list, char **cmd, int cmd_type)
{
	t_cmd_node	*new_node;

	new_node = ft_malloc(sizeof(t_cmd_node), 1);
	if (!new_node)
		return ((void) NULL);
	new_node->next = NULL;
	new_node->cmd_type = cmd_type;
	new_node->cmd = cmd;
	new_node->files = ft_malloc(sizeof(t_file_list), 1);
	if (!new_node->files)
	{
		free(new_node);
		return ;
	}
	new_node->files->head = NULL;
	new_node->files->tail = NULL;
	new_node->files->size = 0;
	if (!cmd_list->head)
		cmd_list->head = new_node;
	else
		cmd_list->tail->next = new_node;
	cmd_list->tail = new_node;
	cmd_list->size++;
}

t_file_node	*create_file_node(char *filename, int redir_type)
{
	t_file_node	*node;

	node = ft_malloc(sizeof(t_file_node), 1);
	if (!node)
		return (NULL);
	node->filename = filename;
	node->redir_type = redir_type;
	node->next = NULL;
	return (node);
}

void	add_file_node(t_file_list *files, t_file_node *node)
{
	if (!files->head)
		files->head = node;
	else
		files->tail->next = node;
	files->tail = node;
	files->size++;
}
