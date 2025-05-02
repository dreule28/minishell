/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:28:29 by dreule            #+#    #+#             */
/*   Updated: 2025/05/02 11:28:30 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_same_quotes(t_token_list *list, char *input, int *i)
{
	char	quote;
	int		token_type;

	quote = input[*i];
	(*i)++;
	if (input[*i] == quote)
	{
		if (quote == '"')
			token_type = D_QUOTES;
		else
			token_type = S_QUOTES;
		if (!ft_strnstr(&input[*i], "\"\"", ft_strlen(input))
			|| !ft_strnstr(&input[*i], "''", ft_strlen(input)))
			add_token(list, "", token_type);
	}
	(*i)++;
	return (1);
}

int	check_quotes(t_token_list *list, char *input, int *i)
{
	int		start;
	char	quote;
	int		token_type;
	char	*token_value;

	quote = input[(*i)];
	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("Syntax error: Open quotes!\n", 2);
		list->syntax_error = 1;
		return (0);
	}
	(*i)++;
	if (quote == '"')
		token_type = D_QUOTES;
	else
		token_type = S_QUOTES;
	token_value = gc_substr(input, start, *i - start);
	add_token(list, token_value, token_type);
	return (1);
}

void	check_and_assign(t_token_list *list, char *input, int *i, int redir_typ)
{
	int	start;

	start = *i;
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		redir_typ = TK_HEREDOC;
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		redir_typ = TK_APPEND;
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		redir_typ = TK_INFILE;
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		redir_typ = TK_OUTFILE;
		(*i)++;
	}
	add_token(list, gc_substr(input, start, *i - start), redir_typ);
}

int	check_redirs(t_token_list *list, char *input, int *i)
{
	int	redir_type;
	int	start;

	start = *i;
	redir_type = 0;
	check_and_assign(list, input, i, redir_type);
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	start = *i;
	if (input[*i] == '"' || input[*i] == '\'')
		check_quotes(list, input, i);
	else
	{
		while (input[*i] && !(input[*i] == ' ' || input[*i] == '\t')
			&& !is_special_char(&input[*i]))
			(*i)++;
		if (start == *i)
		{
			ft_putstr_fd("Syntax error: Missing filename!\n", 2);
			list->syntax_error = 1;
			return (0);
		}
		add_token(list, gc_substr(input, start, *i - start), TK_WORD);
	}
	return (1);
}

void	handle_pipe(t_token_list *list, int *i)
{
	add_token(list, "|", TK_PIPE);
	(*i)++;
}
