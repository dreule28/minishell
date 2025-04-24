#include "minishell.h"

char	*get_type(char *str)
{
	int		count;
	char	*type;

	count = 0;
	while (str[count] != '\0' && str[count] != '=' && str[count] != '+')
		count++;
	type = ft_malloc(count + 1, sizeof(char));
	ft_strlcpy(type, str, count + 1);
	return (type);
}

char	*get_value(char *str)
{
	int		count;
	int		lenght;
	char	*value;

	count = 0;
	lenght = 0;
	while (str[count] != '\0' && str[count] != '=')
		count++;
	while (str[count] == '=')
		count++;
	lenght = ft_strlen(str + count);
	value = ft_malloc(lenght + 1, sizeof(char));
	ft_strlcpy(value, str + count, lenght + 1);
	return (value);
}

int	check_duplicates(char *str, t_env_list *env_list)
{
	char		*type;
	char		*value;
	t_env_node	*env_node;

	env_node = env_list->head;
	type = get_type(str);
	value = get_value(str);
	while (env_node)
	{
		if (ft_strcmp(type, env_node->type) == 0)
		{
			if(value[0] == '\0' && ft_strchr(str, '=') == NULL)
				return(-1);
			env_node->value = value;
			return (-1);
		}
		env_node = env_node->next;
	}
	return (0);
}

int	add_or_attach(char *str)
{
	int	count;

	count = 0;
	while (str[count] != '\0' && str[count] != '=')
		count++;
	if (count > 0 && str[count] == '=' && str[count - 1] == '+')
		return (-1);
	return (0);
}

int	export_attach(char *str, t_env_list *env_list)
{
	char		*type;
	char		*value;
	char		*attached_string;
	t_env_node	*env_node;

	env_node = env_list->head;
	type = get_type(str);
	value = get_value(str);
	while (env_node != NULL)
	{
		if (ft_strcmp(type, env_node->type) == 0)
		{
			attached_string = gc_strjoin(env_node->value, value);
			env_node->value = attached_string;
			return (-1);
		}
		env_node = env_node->next;
	}
	return (0);
}
