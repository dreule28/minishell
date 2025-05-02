/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:28:20 by dreule            #+#    #+#             */
/*   Updated: 2025/05/02 11:28:21 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (!ft_strcmp(lower_str, "echo"))
		return (TK_BUILTIN);
	if (!ft_strcmp(lower_str, "cd"))
		return (TK_BUILTIN);
	if (!ft_strcmp(lower_str, "pwd"))
		return (TK_BUILTIN);
	if (!ft_strcmp(lower_str, "export"))
		return (TK_BUILTIN);
	if (!ft_strcmp(lower_str, "unset"))
		return (TK_BUILTIN);
	if (!ft_strcmp(lower_str, "env"))
		return (TK_BUILTIN);
	if (!ft_strcmp(lower_str, "exit"))
		return (TK_BUILTIN);
	return (TK_WORD);
}
