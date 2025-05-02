/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:28:11 by dreule            #+#    #+#             */
/*   Updated: 2025/05/02 11:28:12 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_found(t_token_list *list, char *input, int *i, int start)
{
	char	*token_value;
	int		builtin_type;

	if (start != *i)
	{
		token_value = gc_substr(input, start, *i - start);
		builtin_type = get_bultin_type(token_value);
		add_token(list, token_value, builtin_type);
	}
}

int	handle_quotes(t_token_list *list, char *input, int *i)
{
	char	quote;

	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("Syntax error: Open quotes!\n", 2);
		list->syntax_error = 1;
		return (0);
	}
	(*i)++;
	return (1);
}

int	is_word_boundary(char *input, int *i)
{
	return (input[*i] == ' ' || input[*i] == '\t'
		|| is_special_char(&input[*i]));
}

int	handle_word_or_arg(t_token_list *list, char *input, int *i)
{
	int		start;
	int		in_word;

	start = *i;
	in_word = 1;
	while (input[*i] && in_word)
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			if (!handle_quotes(list, input, i))
				return (0);
		}
		else if (is_word_boundary(input, i))
			in_word = 0;
		else
			(*i)++;
	}
	token_found(list, input, i, start);
	return (1);
}

t_token_list	*lexer(char *input)
{
	t_token_list	*list;
	int				i;

	i = 0;
	list = init_token_list(input);
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (is_empty_quotes(input, i))
			check_same_quotes(list, input, &i);
		else if (is_redir(&input[i]))
			check_redirs(list, input, &i);
		else if (input[i] == '|')
			handle_pipe(list, &i);
		else
			handle_word_or_arg(list, input, &i);
		if (list->syntax_error)
			return (*exit_code() = 258, NULL);
	}
	return (list);
}
