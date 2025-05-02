/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:28:05 by dreule            #+#    #+#             */
/*   Updated: 2025/05/02 11:28:06 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_segment_to_list(t_segment_list *list, char *value, t_seg_type type)
{
	t_segment	*new_segment;

	if (!list || !value)
		return ;
	new_segment = create_segment(value, type);
	if (!new_segment)
		return ;
	if (!list->head)
		list->head = new_segment;
	else
		list->tail->next = new_segment;
	list->tail = new_segment;
	list->size++;
}

void	add_segment_to_token(t_token *token, char *value, t_seg_type type)
{
	if (!token || !value)
		return ;
	if (!token->segment_list)
	{
		token->segment_list = init_segment_list();
		if (!token->segment_list)
			return ;
	}
	add_segment_to_list(token->segment_list, value, type);
}

void	split_token_into_segments(t_token *token)
{
	int	i;
	int	start;

	i = 0;
	start = 0;
	if (!token || !token->value)
		return ;
	while (token->value[i])
	{
		if (handle_segments(token, &i, &start))
			continue ;
		if (handle_single_quotes(token, &i, &start))
			continue ;
		if (handle_double_quotes(token, &i, &start))
			continue ;
		i++;
	}
	if (i > start)
		add_segment_to_token(token, gc_substr(token->value, start, i - start),
			SEG_NO_QUOTE);
}

void	split_token_into_heredocs(t_token *token)
{
	int	i;
	int	start;

	start = 0;
	if (!token || !token->value)
		return ;
	i = ft_strlen(token->value);
	if (i > start)
		add_segment_to_token(token, gc_substr(token->value, start, i - start),
			SEG_NO_QUOTE);
}

t_token_list	*process_token_list(t_token_list *token_list)
{
	t_token	*curr;

	if (!token_list || !token_list->head)
		return (NULL);
	curr = token_list->head;
	while (curr)
	{
		split_token_into_segments(curr);
		curr = curr->next;
	}
	return (token_list);
}
