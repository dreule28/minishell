
#include "minishell.h"

t_token_list	*init_token_list()
{
	t_token_list	*list;

	list = ft_malloc(sizeof(t_cmd_list), 1);
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->input = NULL;
	return (list);
}

void	add_token(t_token_list *list, char *token, int type)
{
	t_token	*new_token;

	new_token = ft_malloc(sizeof(t_token), 1);
	if (!new_token)
		return ;
	new_token->next = NULL;
	new_token->token = type;
	new_token->value = token;
	if (!list->head)
		list->head = new_token;
	else
		list->tail->next = new_token;
	list->tail = new_token;
	list->size++;
}
