#include "minishell.h"

bool	is_redirection(int token_type)
{
	return (token_type == TK_INFILE || token_type == TK_OUTFILE
		|| token_type == TK_HEREDOC || token_type == TK_APPEND);
}

bool	check_command(int token_type)
{
	return (token_type == TK_WORD || token_type == TK_BUILTIN
		|| token_type == S_QUOTES || token_type == D_QUOTES
		|| token_type == TK_AMPERSAND || token_type == TK_AND
		|| token_type == TK_OR);
}

void	process_cmd_node(t_cmd_list *cmd_list, t_token *token, int cmd_type)
{
	char	**cmd;

	if (!cmd_list->tail)
	{
		DEBUG_INFO("No tail exists, creating new command node for: %s", token->value);
		cmd = ft_malloc(sizeof(char *) * 2, 1);
		if (!cmd)
		{
			DEBUG_ERROR("Memory allocation failed for command array");
			return;
		}
		cmd[0] = gc_strdup(token->value);
		if (!cmd[0])
		{
			DEBUG_ERROR("Failed to duplicate command value");
			free(cmd);
			return;
		}
		cmd[1] = NULL;
		if (!add_cmd_node(cmd_list, cmd, cmd_type))
		{
			DEBUG_ERROR("Failed to add command node");
			free(cmd[0]);
			free(cmd);
			return;
		}
		DEBUG_INFO("Added new command node with command: %s", cmd[0]);
	}
	else
	{
		DEBUG_INFO("Appending to existing command");
		append_arg(cmd_list, token);
	}
}

void	handle_pipe_node(t_cmd_list *cmd_list, t_token *token, t_token **curr)
{
	int	next_cmd_type;

	next_cmd_type = CMD;
	if (token->next && token->next->token == TK_BUILTIN)
		next_cmd_type = BUILTIN;
	add_cmd_node(cmd_list, NULL, next_cmd_type);
	if (token->next)
		*curr = token->next;
}

void	process_token(t_cmd_list *cmd_list, t_token **curr)
{
	t_token	*token;

	token = *curr;
	DEBUG_TRACE("Processing token: [%s] (type: %d)", token->value, token->token);
	if (token->token == TK_PIPE && !token->next)
	{
		ft_putstr_fd("Syntax error: Empty Pipe", 2);
		*curr = NULL;
		return ;
	}
	else if (token->token == TK_PIPE)
	{
		handle_pipe_node(cmd_list, token, curr);
	}
	else if (is_redirection(token->token))
	{
		DEBUG_INFO("Handling redirection: %s", token->value);
		handle_redirection(cmd_list, token);
		if (token->next)
			*curr = token->next;
	}
	else
	{
		DEBUG_INFO("Handling command/argument: %s", token->value);
		handle_command(cmd_list, token);
		*curr = token->next;
	}
}
