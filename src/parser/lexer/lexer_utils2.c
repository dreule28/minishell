#include "minishell.h"

bool	is_empty_quotes(char *input, int i)
{
	if (input[i] == '"' && input[i + 1] == '"'
		&& (input[i + 2] == ' ' || input[i + 2] == '\t' || !input[i + 2]))
		return (true);
	if (input[i] == '\'' && input[i + 1] == '\''
		&& (input[i + 2] == ' ' || input[i + 2] == '\t' || !input[i + 2]))
		return (true);
	return (false);
}
