#include "minishell.h"

t_cmd_list	*init_token_list()
{
	t_cmd_list	*list;

	list = ft_malloc(sizeof(t_cmd_list), 1);
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

t_cmd_node	*add_cmd_node(int cmd_type)
{
	t_cmd_node	*new_node;
	t_file_list	*list;

	new_node = ft_malloc(sizeof(t_cmd_node), 1);
	if (!new_node)
		return ((t_cmd_node *)NULL);
	list = ft_malloc(sizeof(t_file_list), 1);
	if (!list)
		return (NULL);
	new_node->cmd = NULL;
	new_node->cmd_type = cmd_type;
	new_node->files = list;
	new_node->next = NULL;
	return (new_node);
}

void	add_end(t_cmd_list *list, t_cmd_node *new_node)
{
	if (!list->head)
	{
		list->head = new_node;
		list->tail = new_node;
		new_node->next = NULL;
		return ;
	}
	new_node->next = NULL;
	list->tail->next = new_node;
	list->tail = new_node;

}

t_cmd_list	*tokenizer(char *input)
{
	t_cmd_list	*list;
	int			i;
	char		*quote;

	i = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i++;
		if (input[i] == '"' || input[i] == '\'')
		{
			quote = &input[i];
			i++;
			while (input[i] && input[i] != *quote)
				i++;
			if (!input[i])
			{
				printf("Error: close the quote!\n");
				return (NULL);
			}
			i++;
		}
		if (input[i] == '|')
		{
			
		}
		i++;
	}
	return (list);
}
