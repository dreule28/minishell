#include "minishell.h"

void	handle_exit_code(t_segment *segment, char *before, int *i)
{
	char	*exit_status;

	(*i)++;
	exit_status = gc_itoa(get_exit_status(0,0));
	segment->value = gc_strjoin(before, exit_status); 		//to be implemented (exit_status)
}

void	handle_rest(t_env_list *env_list, char *before, t_segment *segment, int *i)
{
	char	*variable_name;
	char	*env_value;
	int		start;

	start = *i + 1;
	(*i)++;
	while (segment->value[(*i)] && ((ft_isalnum(segment->value[(*i)]))
			|| segment->value[(*i)] == '_'))
		(*i)++;
	variable_name = gc_substr(segment->value, start, (*i) - start);
	env_value = get_env_value(env_list, variable_name);
	segment->value = gc_strjoin(before, env_value);
	*i = ft_strlen(before);
}

void	expand_segment(t_segment *segment, t_env_list *env_list)
{
	int		i;
	char	*before;

	if (!segment || !segment->value || !env_list)
		return ;
	if (segment->type == SEG_SINGLE)
		return ;
	i = 0;
	while (segment->value[i])
	{
		if (segment->value[i] == '$' && segment->value[i + 1])
		{
			before = gc_substr(segment->value, 0, i);
			if (segment->value[i + 1] == '?')
				handle_exit_code(segment, before, &i);
			else
				handle_rest(env_list, before, segment, &i);
		}
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
