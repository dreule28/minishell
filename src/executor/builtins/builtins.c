#include "minishell.h"

bool	is_all_flag(char *flag, char c)
{
	int	i;

	if (!flag || flag[0] != '-')
		return (false);
	i = 1;
	if (!flag[i])
		return (false);
	while (flag[i])
	{
		if (flag[i] != c)
			return (false);
		i++;
	}
	return (true);
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

void builtin_exit(t_cmd_node *cmd_node, t_env_list *env_list)
{
	(void)cmd_node;
	(void)env_list;
}
