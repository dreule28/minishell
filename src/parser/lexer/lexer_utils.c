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

bool	is_special_char(char *c)
{
	return (*c == '|' || *c == '>' || *c == '<');
}

char	*turn_lower(char *cmd)
{
	int		i;
	char	*lower_str;

	i = 0;
	lower_str = gc_strdup(cmd);
	while (lower_str[i])
	{
		lower_str[i] = ft_tolower(lower_str[i]);
		i++;
	}
	return (lower_str);
}

int	get_bultin_type(char *cmd)
{
	char	*lower_str;

	if (!cmd)
		return (TK_WORD);
	lower_str = turn_lower(cmd);
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
