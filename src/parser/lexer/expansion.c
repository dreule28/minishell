/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:28:23 by dreule            #+#    #+#             */
/*   Updated: 2025/05/02 14:11:27 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_code(t_segment *segment, char *before, int *i)
{
	char	*after;

	(*i) += 2;
	after = gc_substr(segment->value, *i, ft_strlen(segment->value) - *i);
	before = gc_strjoin(before, gc_itoa(*exit_code()));
	segment->value = gc_strjoin(before, after);
	*i = ft_strlen(before);
}

void	handle_rest(t_env_list *env_list, char *before, t_segment *segment
			, int *i)
{
	char	*variable_name;
	char	*env_value;
	char	*after;
	int		start;

	start = *i + 1;
	(*i)++;
	while (segment->value[(*i)] && ((ft_isalnum(segment->value[(*i)]))
			|| segment->value[(*i)] == '_'))
		(*i)++;
	variable_name = gc_substr(segment->value, start, (*i) - start);
	env_value = get_env_value(env_list, variable_name);
	if (!env_value)
		env_value = "";
	after = gc_substr(segment->value, *i, ft_strlen(segment->value) - *i);
	before = gc_strjoin(before, env_value);
	segment->value = gc_strjoin(before, after);
	*i = ft_strlen(before);
}

void	expand_segment(t_segment *segment, t_env_list *env_list)
{
	int		i;
	char	*before;

	if (!segment || !segment->value || segment->type == SEG_SINGLE || !env_list)
		return ;
	i = 0;
	while (segment->value[i])
	{
		before = gc_substr(segment->value, 0, i);
		if (segment->value[i] == '$' && segment->value[i + 1]
			&& segment->value[i + 1] != ' ' && segment->value[i + 1] != '/')
		{
			if (segment->value[i + 1] == '?')
				handle_exit_code(segment, before, &i);
			else
				handle_rest(env_list, before, segment, &i);
		}
		else if ((segment->type != SEG_DOUBLE && segment->type != SEG_SINGLE)
			&& (segment->value[i] == '~' && !segment->value[i + 1] && !i))
			segment->value = gc_strjoin(before, "$HOME");
		else
			i++;
	}
}

void	expand_token(t_token *token, t_env_list *env_list)
{
	t_segment	*segment;

	if (!token || !token->value)
		return ;
	if (token->segment_list && token->segment_list->head)
	{
		segment = token->segment_list->head;
		while (segment)
		{
			expand_segment(segment, env_list);
			segment = segment->next;
		}
	}
	rebuild_token_value(token);
}

void	segment_tokens(t_token_list *tokens, t_env_list *env_list)
{
	t_token	*curr;

	if (!tokens || !env_list)
		return ;
	curr = tokens->head;
	while (curr)
	{
		if (!curr->segment_list || !curr->segment_list->head)
			split_token_into_segments(curr);
		expand_token(curr, env_list);
		curr = curr->next;
	}
}
