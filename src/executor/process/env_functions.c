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
	ft_putstr_fd("PATH variable not found", 2);
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

char	**env_converter(t_env_list *env_list)
{
	int			count;
	t_env_node	*env_node;
	char		**converted_env_list;
	char *tmp;
	char *env_str;

	count = 0;
	env_node = env_list->head;
	count = env_lenght(env_list);
	converted_env_list = ft_malloc(count + 1, sizeof(char *));
	if (!converted_env_list)
		return (ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
	env_node = env_list->head;
	count = 0;
	while (env_node)
	{
		tmp = ft_strjoin(env_node->type, "=");
		env_str = gc_strjoin(tmp, env_node->value);
		free(tmp);
		if (!env_str)
			return (ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
		converted_env_list[count++] = env_str;
		env_node = env_node->next;
	}
	converted_env_list[count] = NULL;
	return (converted_env_list);
}
char	**env_converter_export(t_env_list *env_list)
{
	int			count;
	t_env_node	*env_node;
	char		**converted_env_list;
	char *tmp;
	char *env_str;

	count = 0;
	env_node = env_list->head;
	count = env_lenght(env_list);
	converted_env_list = ft_malloc(count + 1, sizeof(char *));
	if (!converted_env_list)
		return (ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
	env_node = env_list->head;
	count = 0;
	while (env_node)
	{
		tmp = ft_strjoin(env_node->type, "=");
		env_str = ft_strjoin(tmp, "\"");
		free(tmp);
		tmp = ft_strjoin(env_str, env_node->value);
		free(env_str);
		env_str = gc_strjoin(tmp, "\"");
		free(tmp);
		if (!env_str)
			return (ft_putstr_fd("Error: memory allocation failed\n", 2), NULL);
		converted_env_list[count++] = env_str;
		env_node = env_node->next;
	}
	converted_env_list[count] = NULL;
	return (converted_env_list);
}
