#include "minishell.h"

t_cmd_list	*init_cmd_list(void)
{
	t_cmd_list	*cmd_list;

	DEBUG_TRACE("Initializing command list");
	cmd_list = ft_malloc(sizeof(t_cmd_list), 1);
	if (!cmd_list)
	{
		DEBUG_ERROR("Failed to allocate memory for command list");
		return (NULL);
	}
	cmd_list->head = NULL;
	cmd_list->tail = NULL;
	cmd_list->size = 0;
	DEBUG_INFO("Command list initialized successfully");
	return (cmd_list);
}

t_cmd_node	*add_cmd_node(t_cmd_list *cmd_list, char **cmd, int cmd_type)
{
	t_cmd_node	*new_node;

	//can be deleted for norm(25 line)
	// if (!cmd_list)
	// {
	// 	DEBUG_ERROR("Cannot add node to NULL command list");
	// 	return (NULL);
	// }
	DEBUG_TRACE("Adding command node of type %d", cmd_type);
	new_node = ft_malloc(sizeof(t_cmd_node), 1);
	if (!new_node)
	{
		DEBUG_ERROR("Failed to allocate memory for command node");
		return (NULL);
	}
	new_node->next = NULL;
	new_node->cmd_type = cmd_type;
	new_node->cmd = cmd;
	new_node->files = ft_malloc(sizeof(t_file_list), 1);
	if (!new_node->files)
	{
		DEBUG_ERROR("Failed to allocate memory for file list");
		free(new_node);
		return (NULL);
	}
	new_node->files->head = NULL;
	new_node->files->tail = NULL;
	new_node->files->size = 0;
	if (!cmd_list->head)
	{
		cmd_list->head = new_node;
		DEBUG_TRACE("Node added as head of list");
	}
	else
	{
		cmd_list->tail->next = new_node;
		DEBUG_TRACE("Node added after tail");
	}
	cmd_list->tail = new_node;
	cmd_list->size++;
	DEBUG_INFO("Command node added successfully (list size: %zu)", cmd_list->size);
	return (new_node);
}

t_file_node	*add_file_node(t_file_list *files, char *filename, int redir_type)
{
	t_file_node	*new_node;

	if (!files)
	{
		DEBUG_ERROR("Cannot add node to NULL file list");
		return (NULL);
	}
	if (!filename)
	{
		DEBUG_ERROR("Cannot add file node with NULL filename");
		return (NULL);
	}
	DEBUG_TRACE("Adding file node: %s (type: %d)", filename, redir_type);
	new_node = ft_malloc(sizeof(t_file_node), 1);
	if (!new_node)
	{
		DEBUG_ERROR("Failed to allocate memory for file node");
		return (NULL);
	}
	new_node->filename = filename;
	new_node->redir_type = redir_type;
	new_node->next = NULL;
	if (!files->head)
	{
		files->head = new_node;
		DEBUG_TRACE("Node added as head of file list");
	}
	else
	{
		files->tail->next = new_node;
		DEBUG_TRACE("Node added after file list tail");
	}
	files->tail = new_node;
	files->size++;
	DEBUG_INFO("File node added successfully (list size: %zu)", files->size);
	return (new_node);
}

t_file_list	*init_file_list(void)
{
	t_file_list	*file_list;

	DEBUG_TRACE("Initializing file list");
	file_list = ft_malloc(sizeof(t_file_list), 1);
	if (!file_list)
	{
		DEBUG_ERROR("Failed to allocate memory for file list");
		return (NULL);
	}
	file_list->head = NULL;
	file_list->tail = NULL;
	file_list->size = 0;
	DEBUG_INFO("File list initialized successfully");
	return (file_list);
}
