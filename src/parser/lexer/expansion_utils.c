#include "minishell.h"

bool	handle_single_quotes(t_token *token, int *i, int *start)
{
	if (token->value[*i] == '\'')
	{
		if ((*i) > (*start))
			add_segment_to_token(token, gc_substr(token->value,
					(*start), (*i) - (*start)), SEG_NO_QUOTE);
		(*start) = (*i) + 1;
		(*i)++;
		while (token->value[(*i)] && token->value[(*i)] != '\'')
			(*i)++;
		if ((*i) > (*start))
			add_segment_to_token(token, gc_substr(token->value,
					(*start), (*i) - (*start)), SEG_SINGLE);
		if (token->value[(*i)] == '\'')
			(*i)++;
		(*start) = (*i);
		return (true);
	}
	return (false);
}

bool	handle_double_quotes(t_token *token, int *i, int *start)
{
	if (token->value[*i] == '"')
	{
		if ((*i) > (*start))
			add_segment_to_token(token, gc_substr(token->value,
					(*start), (*i) - (*start)), SEG_NO_QUOTE);
		(*start) = (*i) + 1;
		(*i)++;
		while (token->value[(*i)] && token->value[(*i)] != '"')
			(*i)++;
		if ((*i) > (*start))
			add_segment_to_token(token, gc_substr(token->value,
					(*start), (*i) - (*start)), SEG_DOUBLE);
		if (token->value[(*i)] == '"')
			(*i)++;
		(*start) = (*i);
		return (true);
	}
	return (false);
}

char	*get_env_value(t_env_list *env_list, char *variable_name)
{
	t_env_node	*curr;
	char		*result;

	if (!env_list || !variable_name || variable_name[0] == '\0')
		return (gc_strdup(""));
	curr = env_list->head;
	while (curr)
	{
		if (!ft_strcmp(curr->type, variable_name))
			return (gc_strdup(curr->value));
		curr = curr->next;
	}
	result = gc_strdup("");
	return (result);
}

void	rebuild_token_value(t_token *token)
{
	t_segment	*segment;
	char		*result;

	if (!token || !token->segment_list || !token->segment_list->head)
		return ;
	segment = token->segment_list->head;
	result = gc_strdup("");
	while (segment)
	{
		result = gc_strjoin(result, segment->value);
		segment = segment->next;
	}
	token->value = result;
}
