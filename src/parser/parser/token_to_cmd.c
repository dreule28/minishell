#include "minishell.h"

void	transfer_cmd(char **new_cmd, char **old_cmd, int *i)
{
	DEBUG_TRACE("Transferring commands from old to new array, starting at index %d", *i);

	while (old_cmd[(*i)])
	{
		new_cmd[(*i)] = gc_strdup(old_cmd[(*i)]);
		if (!new_cmd[(*i)])
		{
			DEBUG_ERROR("Memory allocation failed during command transfer");
			return ;
		}
		(*i)++;
	}
	DEBUG_TRACE("Successfully transferred %d command arguments", *i);
}

int	handle_redirection(t_cmd_list *cmd_list, t_token *token)
{
	t_cmd_node	*cmd_node;
	t_file_node	*file_node;

	if (!cmd_list || !token || !token->next)
	{
		DEBUG_ERROR("Invalid parameters in handle_redirection");
		cmd_list->syntax_error = 1;
		return (0);
	}
	DEBUG_TRACE("Handling redirection of type %d, target: %s", token->token, token->next->value);
	if (!cmd_list->tail)
	{
		DEBUG_INFO("No command node exists, creating new one");
		cmd_node = add_cmd_node(cmd_list, NULL, CMD);
		if (!cmd_node)
		{
			DEBUG_ERROR("Failed to create command node for redirection");
			return (0);
		}
	}
	else
		cmd_node = cmd_list->tail;
	file_node = add_file_node(cmd_node->files, token->next->value, token->token);
	if (!file_node)
	{
		DEBUG_ERROR("Failed to add file node for redirection");
		return (0);
	}
	DEBUG_INFO("Added redirection for file: %s with type %d", token->next->value, token->token);
	return (1);
}

void	append_arg(t_cmd_list *cmd_list, t_token *token)
{
	char	**new_cmd;
	char	**old_cmd;
	int		i;

	if (!cmd_list || !cmd_list->tail || !cmd_list->tail->cmd || !token)
	{
		DEBUG_ERROR("Invalid parameters in append_arg");
		return ;
	}
	DEBUG_TRACE("Appending argument: %s", token->value);
	i = 0;
	while (cmd_list->tail->cmd[i])
		i++;
	DEBUG_TRACE("Current command has %d arguments, allocating for %d", i, i + 2);
	new_cmd = ft_malloc(sizeof(char *), (i + 2));
	if (!new_cmd)
	{
		DEBUG_ERROR("Memory allocation failed for new command array");
		return ;
	}
	old_cmd = cmd_list->tail->cmd;
	i = 0;
	transfer_cmd(new_cmd, old_cmd, &i);
	new_cmd[i] = gc_strdup(token->value);
	if (!new_cmd[i])
	{
		DEBUG_ERROR("Failed to duplicate token value");
		cmd_list->tail->cmd = old_cmd;
		return;
	}
	new_cmd[i + 1] = NULL;
	cmd_list->tail->cmd = new_cmd;
	DEBUG_INFO("Argument successfully appended");
}

void	handle_command(t_cmd_list *cmd_list, t_token *token)
{
	int		cmd_type;
	char	**cmd;

	if (!cmd_list || !token)
	{
		DEBUG_ERROR("Invalid parameters in handle_command");
		return ;
	}
	DEBUG_TRACE("Handling command token: %s", token->value);
	if (token->token == TK_BUILTIN)
	{
		cmd_type = BUILTIN;
		DEBUG_INFO("Detected builtin command: %s", token->value);
	}
	else
		cmd_type = CMD;
	if (cmd_list->tail && !cmd_list->tail->cmd)
	{
		DEBUG_INFO("Creating new command node for: %s", token->value);
		cmd = ft_malloc(sizeof(char *) * 2, 1);
		if (!cmd)
		{
			DEBUG_ERROR("Memory allocation failed for command array");
			return ;
		}
		cmd[0] = gc_strdup(token->value);
		if (!cmd[0])
		{
			DEBUG_ERROR("Failed to duplicate command value");
			return ;
		}
		cmd[1] = NULL;
		cmd_list->tail->cmd = cmd;
		DEBUG_INFO("Command assigned: %s", cmd[0]);
	}
	else
		process_cmd_node(cmd_list, token, cmd_type);
}

t_cmd_list	*token_to_cmd(t_token_list *tk_list)
{
	t_cmd_list	*cmd_list;
	t_token		*curr;

	DEBUG_INFO("Starting token to command conversion");
	if (!tk_list || !tk_list->head)
	{
		DEBUG_ERROR("Empty token list");
		return (NULL);
	}
	cmd_list = init_cmd_list();
	if (!cmd_list)
	{
		DEBUG_ERROR("Failed to initialize command list");
		return (NULL);
	}
	curr = tk_list->head;
	while (curr)
	{
		process_token(cmd_list, &curr);
		if (cmd_list->syntax_error)
		{
			// ft_putstr_fd("Syntax error: Learn syntax lol", 2);
			DEBUG_ERROR("Syntax error detected, stopping token_to_cmd processing");
			return (0);
		}
	}
	DEBUG_INFO("Token to command conversion complete");
	validate_cmd_list(cmd_list);
	return (cmd_list);
}

// ls -l | grep .txt | echo "hello" | wc -l | < input