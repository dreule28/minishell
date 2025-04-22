#include "minishell.h"


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
	value = ft_malloc(lenght + 1 , sizeof(char));
	ft_strlcpy(value, str + count, lenght + 1);
	return(value);
}

int	check_duplicates(t_cmd_node *cmd_node, t_env_list *env_list)
{
	char *type;
	char *value;
	t_env_node *env_node;

	env_node = env_list->head;
	type = get_type(cmd_node->cmd[1]);
	value = get_value(cmd_node->cmd[1]);
	while(env_node)
	{
		if(ft_strcmp(type, env_node->type) == 0)
		{
			env_node->value = value;
			return (-1);
		}
		env_node = env_node->next;
	}
	return (0);
}

void export_variable(t_cmd_node *cmd_node, t_env_list *env_list)
{
	char *type;
	char *value;

	if(check_duplicates(cmd_node, env_list) != -1)
	{
		type = get_type(cmd_node->cmd[1]);
		value = get_value(cmd_node->cmd[1]);
		add_env_node(env_list, type, value);
	}
}

void	print_export_variables(t_env_list *env_list)
{
	int count;
	char **converted_env_list;

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
