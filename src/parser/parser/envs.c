#include "minishell.h"

t_env_list	*init_env_list(void)
{
	t_env_list	*new_list;

	new_list = ft_malloc(sizeof(t_env_list), 1);
	if (!new_list)
		return (NULL);
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->size = 0;
	return (new_list);
}

t_env_node	*add_env_node(char *type, char *value)
{
	t_env_node	*new_node;

	new_node = ft_malloc(sizeof(t_env_node), 1);
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->key = type;
	new_node->value = value;
	return (new_node);
}

char	*find_equal_sign(char **env, int i, char **value)
{
	char	*type;
	char	*equal_pos;
	int		len;

	equal_pos = ft_strchr(env[i], '=');
	if (!equal_pos)
		return (gc_strdup(env[i]));
	len = equal_pos - env[i];
	type = gc_substr(env[i], 0, len);
	*value = gc_strdup(equal_pos + 1);
	return (type);
}

t_env_list	*get_envs(char **env)
{
	t_env_list	*env_list;
	t_env_node	*new_node;
	int			i;
	char		*type;
	char		*value;

	i = 0;
	env_list = init_env_list();
	while (env[i])
	{
		type = find_equal_sign(env, i, &value);
		if (type)
		{
			new_node = add_env_node(type, value);
			if (!env_list->head)
				env_list->head = new_node;
			else
				env_list->tail->next = new_node;
			env_list->tail = new_node;
			env_list->size++;
		}
		i++;
	}
	return (env_list);
}
