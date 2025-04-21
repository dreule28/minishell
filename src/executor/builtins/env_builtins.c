#include "minishell.h"

void	builtin_env(t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node != NULL)
	{
		if (env_node->value != NULL)
		{
			ft_putstr_fd(env_node->type, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(env_node->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env_node = env_node->next;
	}
}

char *get_type(char *str)
{
	int count; 
	char *type;

	count = 0;
	while(str[count] != '\0' && str[count] != '=')
		count++;
	type = ft_malloc(count + 1, sizeof(char));
	ft_strlcpy(type, str, count + 1);
	return(type);
}

char *get_value(char *str)
{
	int count;
	int lenght;
	char *value;

	count = 0;
	lenght = 0;
	while(str[count] != '\0' && str[count] != '=')
		count++;
	while(str[count] == '=')
		count++;
	lenght = ft_strlen(str + count);
	value = ft_malloc(lenght , sizeof(char));
	ft_strlcpy(value, str + count, lenght + 1);
	return(value);
}


void	builtin_export(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int count;
	char *type;
	char *value;
	char **converted_env_list;
	
	if(cmd_node->cmd[1] == NULL)
	{
		count = 0;
		converted_env_list = env_converter_export(env_list);
		sort_env_array(converted_env_list);
		while(converted_env_list[count] != NULL)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(converted_env_list[count], 1);
			ft_putstr_fd("\n", 1);
			count++;
		}	
	}
	else
	{
		type = get_type(cmd_node->cmd[1]);
		value = get_value(cmd_node->cmd[1]);
		DEBUG_INFO("type : %s\n", type);

		DEBUG_INFO("value : %s\n", value);
		add_env_node(type, value);
	}
}

void	builtin_unset(t_cmd_node *cmd_node, t_env_list *env_list)
{
	(void)cmd_node;
	(void)env_list;
}