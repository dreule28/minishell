#include "minishell.h"

bool	is_redir(char *str)
{
	if (!str || !*str)
		return (false);
	if (str[0] == '<' && str[1] && str[1] == '<')
		return (true);
	if (str[0] == '>' && str[1] && str[1] == '>')
		return (true);
	if (str[0] == '<' || str[0] == '>')
		return (true);
	return (false);
}

int	get_redir_type(char *input, int i)
{
	if (!input || !input[i])
		return (0);
	if (input[i] == '<' && input[i + 1] == '<')
		return (TK_HEREDOC);
	if (input[i] == '>' && input[i + 1] == '>')
		return (TK_APPEND);
	if (input[i] == '<')
		return (TK_INFILE);
	if (input[i] == '>')
		return (TK_OUTFILE);
	return (0);
}

bool	is_special_char(const char *c)
{
	return (*c == '|' || *c == '>' || *c == '<');
}

int	get_bultin_type(const char *str)
{
	if (!ft_strncmp(str, "echo", 4))
		return (TK_BUILTIN);
	if (!ft_strncmp(str, "cd", 2))
		return (TK_BUILTIN);
	if (!ft_strncmp(str, "pwd", 3))
		return (TK_BUILTIN);
	if (!ft_strncmp(str, "export", 6))
		return (TK_BUILTIN);
	if (!ft_strncmp(str, "unset", 5))
		return (TK_BUILTIN);
	if (!ft_strncmp(str, "env", 3))
		return (TK_BUILTIN);
	if (!ft_strncmp(str, "exit", 4))
		return (TK_BUILTIN);
	return (TK_WORD);
}
