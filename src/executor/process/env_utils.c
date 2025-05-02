#include "minishell.h"

char	*env_search_path_var(t_env_list *env_list)
{
	t_env_node	*node;

	node = env_list->head;
	while (node->next != NULL)
	{
		if (ft_strncmp(node->type, "PATH", 4) == 0)
			return (node->value);
		node = node->next;
	}
	ft_putstr_fd("PATH variable not found\n", 2);
	return (NULL);
}

int	env_lenght(t_env_list *env_list)
{
	int			count;
	t_env_node	*node;

	count = 0;
	node = env_list->head;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}
