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
		cmd = ft_malloc(sizeof(char *) * 2, 1);
		if (!cmd)
			return;
		cmd[0] = gc_strdup(token->value);
		if (!cmd[0])
			return;
		cmd[1] = NULL;
		if (!add_cmd_node(cmd_list, cmd, cmd_type))
			return;
	}
	else
		append_arg(cmd_list, token);
}

void	process_token(t_cmd_list *cmd_list, t_token **curr)
{
	t_token	*token;

	token = *curr;
	if (token->token == TK_PIPE && (!token->next || cmd_list->size == 0))
	{
		ft_putstr_fd("Syntax error: Empty Pipe from first check\n", 2);
		cmd_list->syntax_error = 1;
		*curr = NULL;
		return ;
	}
	else if (is_redirection(token->token) && !cmd_list->syntax_error)
	{
		handle_redirection(cmd_list, token);
		if (token->next )
			*curr = token->next->next;
		else
			*curr = token->next;
	}
	else
	{
		handle_command(cmd_list, token);
		*curr = token->next;
	}
}
