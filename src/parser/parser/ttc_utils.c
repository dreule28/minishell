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
	t_type	redir_type;

	(void)cmd_type;
	if (!cmd_list->tail)
	{
		cmd = ft_malloc(sizeof(char *) * 2, 1);
		if (!cmd)
			return;
		redir_type = is_redirection(token->token);
		if (token->token == redir_type)
		{
			// DEBUG_INFO("FROM ttc_utils.c-> redirtype: %s\n", token->value);
			cmd[0] = gc_strdup(token->value);
			if (!cmd[0])
				return;
			cmd[1] = NULL;
			if (!add_cmd_node(cmd_list, cmd, cmd_type))
				return;
		}
		// DEBUG_INFO("CMDLIST SIZE: %ld\n", cmd_list->size);
	}
	else
		append_arg(cmd_list, token);
}

int	handle_pipe_node(t_cmd_list *cmd_list, t_token *token, t_token **curr)
{
	int	next_cmd_type;

	if (token->next->token == TK_PIPE)
	{
		cmd_list->syntax_error = 1;
		return (0);
	}
	next_cmd_type = CMD;
	if (token->next && token->next->token == TK_BUILTIN)
		next_cmd_type = BUILTIN;
	add_cmd_node(cmd_list, NULL, next_cmd_type);
	if (token->next)
		*curr = token->next;
	return (1);
}

void	process_token(t_cmd_list *cmd_list, t_token **curr)
{
	t_token	*token;

	token = *curr;
	if (token->token == TK_PIPE && (!token->next || cmd_list->size == 0))
	{
		ft_putstr_fd("Syntax error: Empty Pipe", 2);
		cmd_list->syntax_error = 1;
		*curr = NULL;
		return ;
	}
	else if (token->token == TK_PIPE)
	{
		handle_pipe_node(cmd_list, token, curr);
	}
	else if (is_redirection(token->token) && !cmd_list->syntax_error)
	{
		handle_redirection(cmd_list, token);
		if (token->next)
			*curr = token->next;
	}
	else
	{
		handle_command(cmd_list, token);
		*curr = token->next;
	}
}
