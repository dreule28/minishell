#include "minishell.h"

void	builtin_env(t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
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
	if(cmd_node->cmd[1] == NULL)
		print_export_variables(env_list);
	else
		export_variable(cmd_node, env_list);
}

void	builtin_unset(t_cmd_node *cmd_node, t_env_list *env_list)
{
	t_env_node	*env_node;
	t_env_node	*prev_env_node;
	int count;

	count = 1;
	while(cmd_node->cmd[count] != NULL)
	{
		if(ft_strchr(cmd_node->cmd[count], '=') != NULL)
		{
			ft_putstr_fd("not a valid identifier\n",1 );
			count++;
			continue;
		}
		env_node = env_list->head;
		prev_env_node = NULL;
		while (env_node != NULL)
		{
			if(ft_strcmp(cmd_node->cmd[count], env_node->type) == 0)
			{
				if(prev_env_node == NULL)
					env_list->head = env_node->next;
				else
					prev_env_node->next = env_node->next;
				env_node = env_node->next;
				break;
			}
			prev_env_node = env_node;
			env_node = env_node->next;
		}
		count++;
	}
}
