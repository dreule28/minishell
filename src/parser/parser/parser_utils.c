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
