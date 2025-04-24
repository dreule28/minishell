#include "minishell.h"

void	builtin_exit_code(void)
{
	int	*code_ptr;

	code_ptr = exit_code();
	printf("%d\n", *code_ptr);
}

void builtin_echo(t_cmd_node *cmd_node)
{
	int	i;
	int	no_flag;

	i = 1;
	no_flag = 1;
	if (cmd_node->cmd[1])
	{
		while (cmd_node->cmd[i] && is_all_flag(cmd_node->cmd[i], 'n'))
		{
			no_flag = 0;
			i++;
		}
		while (cmd_node->cmd[i])
		{
			printf("%s ", cmd_node->cmd[i]);
			i++;
		}
		if (no_flag)
			printf("\n");
	}
}

void builtin_cd(t_cmd_node *cmd_node)
{
	(void)cmd_node;
}

void builtin_pwd(t_cmd_node *cmd_node)
{
	(void)cmd_node;
	char *pwd;

	pwd = getcwd(NULL, 0);
	if(pwd != NULL)
	{
		ft_putstr_fd(pwd, 1);
		free(pwd);
		ft_putstr_fd("\n", 1);
	}
}

void builtin_exit(t_cmd_node *cmd_node)
{
	int	exit_val;

	if (cmd_node->cmd[0] || (cmd_node->cmd[0] && cmd_node->cmd[1]))
	{
		if (cmd_node->cmd[0] && cmd_node->cmd[1])
		{
			exit_val = ft_atoi(cmd_node->cmd[1]);
			ft_putstr_fd("exit\n", 2);
			clean_up();
			*exit_code() = exit_val;
			return ;
		}
		else if (cmd_node->cmd[0])
		{
			ft_putstr_fd("exit\n", 2);
			clean_up();
			return ;
		}
	}
}
