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

int	get_bultin_type(const char *cmd)
{
	int		i;
	char	*lower_str;

	if (!cmd)
		return (TK_WORD);
	lower_str = gc_strdup(cmd);
	if (!lower_str)
		return (TK_WORD);
	i = 0;
	while (lower_str[i])
	{
		lower_str[i] = ft_tolower(lower_str[i]);
		i++;
	}
	if (!ft_strncmp(lower_str, "echo", ft_strlen(lower_str)))
		return (TK_BUILTIN);
	if (!ft_strncmp(lower_str, "cd", ft_strlen(lower_str)))
		return (TK_BUILTIN);
	if (!ft_strncmp(lower_str, "pwd", ft_strlen(lower_str)))
		return (TK_BUILTIN);
	if (!ft_strncmp(lower_str, "export", ft_strlen(lower_str)))
		return (TK_BUILTIN);
	if (!ft_strncmp(lower_str, "unset", ft_strlen(lower_str)))
		return (TK_BUILTIN);
	if (!ft_strncmp(lower_str, "env", ft_strlen(lower_str)))
		return (TK_BUILTIN);
	if (!ft_strncmp(lower_str, "exit", ft_strlen(lower_str)))
		return (TK_BUILTIN);
	return (TK_WORD);
}
