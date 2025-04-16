#include "minishell.h"

t_cmd_list	*init_cmd_list(void)
{
	t_cmd_list	*cmd_list;

	cmd_list = ft_malloc(sizeof(t_cmd_list), 1);
	if (!cmd_list)
		return (NULL);
	cmd_list->head = NULL;
	cmd_list->tail = NULL;
	cmd_list->size = 0;
	return (cmd_list);
}

t_cmd_node	*add_cmd_node(t_cmd_list *cmd_list, char **cmd, int cmd_type)
{
	t_cmd_node	*new_node;

	new_node = ft_malloc(sizeof(t_cmd_node), 1);
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->cmd_type = cmd_type;
	new_node->cmd = cmd;
	new_node->files = init_file_list();
	if (!cmd_list->head)
		cmd_list->head = new_node;
	else
		cmd_list->tail->next = new_node;
	cmd_list->tail = new_node;
	cmd_list->size++;
	return (new_node);
}

t_file_node	*add_file_node(t_file_list *files, char *filename, int redir_type)
{
	t_file_node	*new_node;

	if (!files || !filename)
		return (NULL);
	new_node = ft_malloc(sizeof(t_file_node), 1);
	if (!new_node)
		return (NULL);
	new_node->filename = filename;
	new_node->redir_type = redir_type;
	new_node->next = NULL;
	if (!files->head)
		files->head = new_node;
	else
		files->tail->next = new_node;
	files->tail = new_node;
	files->size++;
	return (new_node);
}

t_file_list	*init_file_list(void)
{
	t_file_list	*file_list;

	file_list = ft_malloc(sizeof(t_file_list), 1);
	if (!file_list)
		return (NULL);
	file_list->head = NULL;
	file_list->tail = NULL;
	file_list->size = 0;
	return (file_list);
}
