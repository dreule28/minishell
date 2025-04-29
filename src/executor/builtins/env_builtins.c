#include "minishell.h"

void	builtin_env(t_cmd_node *cmd_node, t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	if (cmd_node->cmd[1] != NULL)
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(cmd_node->cmd[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return ;
	}
	while (env_node != NULL)
	{
		if (env_node->value != NULL && env_node->value[0] != '\0')
		{
			ft_putstr_fd(env_node->type, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(env_node->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env_node = env_node->next;
	}
}

void	builtin_export(t_cmd_node *cmd_node, t_env_list *env_list)
{
	if (cmd_node->cmd[1] == NULL)
		print_export_variables(env_list);
	else
		export_variable(cmd_node, env_list);
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (-1);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (-1);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

void	builtin_unset(t_cmd_node *cmd_node, t_env_list *env_list)
{
	t_env_node	*env_node;
	t_env_node	*prev_env_node;
	int			count;
	int			invalid_found;

	invalid_found = 0;
	count = 1;
	while (cmd_node->cmd[count] != NULL)
	{
		if (is_valid_identifier(cmd_node->cmd[count]) == -1)
		{
			invalid_found = 1;
			ft_putstr_fd("not a valid identifier\n", 2);
			count++;
			continue ;
		}
		env_node = env_list->head;
		prev_env_node = NULL;
		while (env_node != NULL)
		{
			if (ft_strcmp(cmd_node->cmd[count], env_node->type) == 0)
			{
				if (prev_env_node == NULL)
					env_list->head = env_node->next;
				else
					prev_env_node->next = env_node->next;
				break ;
			}
			prev_env_node = env_node;
			env_node = env_node->next;
		}
		count++;
	}
	*exit_code() = invalid_found;
}
