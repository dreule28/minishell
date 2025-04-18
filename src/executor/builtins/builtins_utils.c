#include "minishell.h"

void	single_builtin_execution(t_cmd_node *cmd_node, t_env_list *env_list)
{
	if(file_redirecting(cmd_node, env_list) == -1)
		return ;
	// if(ft_strcmp(cmd_node->cmd[0], "echo") == 0)
	// 	builtin_echo(cmd_node, env_list);
	// else if(ft_strcmp(cmd_node->cmd[0], "cd") == 0)
	// 	builtin_cd(cmd_node);
	// else if(ft_strcmp(cmd_node->cmd[0], "pwd") == 0)
	// 	builtin_pwd(cmd_node);
	// else if(ft_strcmp(cmd_node->cmd[0], "export") == 0)
	// 	builtin_export(cmd_node, env_list);
	// else if(ft_strcmp(cmd_node->cmd[0], "unset") == 0)
	// 	builtin_unset(cmd_node, env_list);
	// else if(ft_strcmp(cmd_node->cmd[0], "env") == 0)
	// 	builtin_env(cmd_node, env_list);
	// else if(ft_strcmp(cmd_node->cmd[0], "exit") == 0)
	// 	builtin_exit(cmd_node);
}