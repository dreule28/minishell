
#include "minishell.h"

bool	is_redir(char *str)
{
	if (!str || !*str)
		return (false);
	if (str[0] == '<' || (str[0] == '<' && str[1] == '<'))
		return (true);
	else if (str[0] == '>' || (str[0] == '>' && str[1] == '>'))
		return (true);
	return (false);
}

int	get_redir_type(char *input, int i)
{
	if (input[i] == '<' && input[i + 1] == '<')
		return (HEREDOC);
	if (input[i] == '>' && input[i + 1] == '>')
		return (APPEND);
	if (input[i] == '<')
		return (OUTFILE);
	if (input[i] == '>')
		return (INFILE);
	return (0);
}

bool	is_special_char(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (true);
	return (false);
}
