#include "minishell.h"

t_token_list	*init_token_list(char *input)
{
	t_token_list	*list;

	list = ft_malloc(sizeof(t_token_list), 1);
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->input = gc_strdup(input);
	if (!list->input)
	{
		free(list);
		return (NULL);
	}
	return (list);
}

void	add_token(t_token_list *list, char *token, int type)
{
	t_token	*new_token;

	if (!token || !*token)
		return ;
	new_token = ft_malloc(sizeof(t_token), 1);
	if (!new_token)
		return ;
	new_token->next = NULL;
	new_token->token = type;
	new_token->value = gc_strdup(token);
	if (!new_token->value)
	{
		free(new_token);
		return ;
	}
	if (!list->head)
		list->head = new_token;
	else
		list->tail->next = new_token;
	list->tail = new_token;
	list->size++;
}
