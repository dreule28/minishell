#include "minishell.h"

void builtin_echo(t_cmd_node *cmd_node, t_env_list *env_list)
{
	(void)cmd_node;
	(void)env_list;
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
	else
		ft_putstr_fd("get_cwd function failed (returned NULL)\n", 2);
}

void builtin_exit(t_cmd_node *cmd_node, t_env_list *env_list)
{
	(void)cmd_node;
	(void)env_list;
}
