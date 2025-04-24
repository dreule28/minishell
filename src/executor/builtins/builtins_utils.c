#include "minishell.h"

void	single_builtin_execution(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int saved_stdin, saved_stdout;
	if (file_redirecting(cmd_node, env_list) == -1)
	{
		reset_redirection(saved_stdin, saved_stdout);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	if (ft_strcmp(cmd_node->cmd[0], "pwd") == 0)
		builtin_pwd(cmd_node);
	else if(ft_strcmp(cmd_node->cmd[0], "cd") == 0)
		builtin_cd(cmd_node, env_list);
	else if (ft_strcmp(cmd_node->cmd[0], "export") == 0)
		builtin_export(cmd_node, env_list);
	else if (ft_strcmp(cmd_node->cmd[0], "unset") == 0)
		builtin_unset(cmd_node, env_list);
	else if (ft_strcmp(cmd_node->cmd[0], "env") == 0)
		builtin_env(cmd_node, env_list);
	else
		ft_putstr_fd("command not found", 2);
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	reset_redirection(saved_stdin, saved_stdout);
	close(saved_stdin);
	close(saved_stdout);
}

void	sort_env_array(char **converted_env_list)
{
	int current;
	int compare;
	char *tmp;

	current = 0;
	while (converted_env_list[current] != NULL)
	{
		compare = current + 1;
		while (converted_env_list[compare] != NULL)
		{
			if (ft_strcmp(converted_env_list[current],
					converted_env_list[compare]) > 0)
			{
				tmp = converted_env_list[current];
				converted_env_list[current] = converted_env_list[compare];
				converted_env_list[compare] = tmp;
			}
			compare++;
		}
		current++;
	}
}
