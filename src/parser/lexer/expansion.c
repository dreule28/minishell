#include "minishell.h"

void	handle_exit_code(t_segment *segment, char *before, int *i)
{
	char	*after;

	DEBUG_TRACE("Handling exit code expansion"); // ADD HERE
	(*i)++;
	before = gc_strjoin(before, gc_itoa(1)); 		//to be implemented (exit_status)
	after = gc_substr(segment->value, *i, ft_strlen(segment->value) - *i);
	segment->value = gc_strjoin(before, after);
}

void	handle_rest(t_env_list *env_list, char *before, t_segment *segment, int *i)
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
	DEBUG_TRACE("Expanding variable: %s", variable_name);
	env_value = get_env_value(env_list, variable_name);
	DEBUG_TRACE("Variable value: %s", env_value);
	after = gc_substr(segment->value, *i, ft_strlen(segment->value) - *i);
	before = gc_strjoin(before, env_value);
	segment->value = gc_strjoin(before, after);
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
	DEBUG_TRACE("Expanding segment: %s", segment->value);
	DEBUG_INFO("Starting segment expansion for: %s", segment->value);
	i = 0;
	while (segment->value[i])
	{
		if (segment->value[i] == '$' && segment->value[i + 1])
		{
			before = gc_substr(segment->value, 0, i);
			if (segment->value[i + 1] == '?')
			{
				handle_exit_code(segment, before, &i);
			}
			else
				handle_rest(env_list, before, segment, &i);
		}
		else
			i++;
	}
	DEBUG_INFO("Finished segment expansion, result: %s", segment->value);
}

void	expand_token(t_token *token, t_env_list *env_list)
{
	t_segment	*segment;

	if (!token || !token->value)
		return ;
	DEBUG_TRACE("Processing token: %s", token->value);
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
	{
		DEBUG_ERROR("Invalid parameters in segment_tokens");
		return ;
	}
	DEBUG_INFO("Starting token segmentation and expansion");
	curr = tokens->head;
	while (curr)
	{
		if (!curr->segment_list || !curr->segment_list->head)
			split_token_into_segments(curr);
		expand_token(curr, env_list);
		curr = curr->next;
	}
	DEBUG_INFO("Token segmentation and expansion complete");
}
