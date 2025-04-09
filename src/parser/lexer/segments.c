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
	{
		DEBUG_ERROR("Null token or token value in parse_token_into_segments");
		return ;
	}
	DEBUG_TRACE("Parsing token into segments: %s", token->value);
	while (token->value[i])
	{
		if (token->value[i] == '\'')
		{
			DEBUG_TRACE("Found single quote at position %d", i);
			handle_single_quotes(token, &i, &start);
			continue ;
		}
		if (token->value[i] == '"')
		{
			DEBUG_TRACE("Found double quote at position %d", i);
			handle_double_quotes(token, &i, &start);
			continue ;
		}
		i++;
	}
	if (i > start)
	{
		DEBUG_TRACE("Adding final segment: [%.*s]", i - start, token->value + start);
		add_segment_to_token(token, gc_substr(token->value, start, i - start), SEG_NO_QUOTE);
	}
	DEBUG_INFO("Segmentation complete. Token now has %zu segments", token->segment_list ? token->segment_list->size : 0);
}

t_token_list	*process_token_list(t_token_list *token_list)
{
	t_token	*curr;

	if (!token_list || !token_list->head)
	{
		DEBUG_ERROR("Null token list or empty list in process_token_list.");
		return (NULL);
	}
	DEBUG_INFO("Starting token list segmentation");
	curr = token_list->head;
	while (curr)
	{
		split_token_into_segments(curr);
		curr = curr->next;
	}
	DEBUG_INFO("Token list segmentation complete");
	return (token_list);
}
