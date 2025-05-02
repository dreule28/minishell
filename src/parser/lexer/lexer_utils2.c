/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:28:17 by dreule            #+#    #+#             */
/*   Updated: 2025/05/02 11:28:18 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

bool	handle_segments(t_token *token, int *i, int *start)
{
	if (token->value[*i] == '$' && (token->value[*i + 1] == '"'
			|| token->value[*i + 1] == '\''))
	{
		if (*i > *start)
			add_segment_to_token(token,
				gc_substr(token->value, *start, *i - *start), SEG_NO_QUOTE);
		(*i)++;
		*start = *i;
		if (token->value[*i] == '\'')
			handle_single_quotes(token, i, start);
		else if (token->value[*i] == '"')
			handle_double_quotes(token, i, start);
		return (true);
	}
	return (false);
}
