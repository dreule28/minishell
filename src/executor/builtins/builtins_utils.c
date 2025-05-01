#include "minishell.h"

int ft_isnum_exit(char *str)
{
	int	count;
	int	has_digit;

	count = 0;
	has_digit = 0;
	while (str[count] == '+' || str[count] == '-')
		count++;
	while (str[count] != '\0')
	{
		if (!(str[count] >= '0' && str[count] <= '9'))
			return (-1);
		has_digit = 1;
		count++;
	}
	if (!has_digit)
		return (-1);
	return (0);
}

int	single_builtin_execution(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int saved_stdin, saved_stdout;


	if(file_redirecting(cmd_node, env_list) == -1) 
	{
		save_stdin_stdout(&saved_stdin, &saved_stdout);
		reset_redirection(saved_stdin, saved_stdout);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	if(ft_strncmp(cmd_node->cmd[0], "pwd", ft_strlen(cmd_node->cmd[0])) == 0)
		builtin_pwd(cmd_node);
	else if(ft_strncmp(cmd_node->cmd[0], "export", ft_strlen(cmd_node->cmd[0])) == 0)
		builtin_export(cmd_node, env_list);
	else if(ft_strncmp(cmd_node->cmd[0], "cd", ft_strlen(cmd_node->cmd[0])) == 0)
		builtin_cd(cmd_node, env_list);
	else if(ft_strncmp(cmd_node->cmd[0], "unset", ft_strlen(cmd_node->cmd[0])) == 0)
		builtin_unset(cmd_node, env_list);
	else if(ft_strncmp(cmd_node->cmd[0], "env", ft_strlen(cmd_node->cmd[0])) == 0)
		builtin_env(cmd_node, env_list);
	else if (ft_strncmp(cmd_node->cmd[0], "echo", ft_strlen(cmd_node->cmd[0])) == 0)
		builtin_echo(cmd_node);
	else if (ft_strncmp(cmd_node->cmd[0], "exit", ft_strlen(cmd_node->cmd[0])) == 0)
	{
		builtin_exit(cmd_node);
		if(cmd_node->cmd[1] == NULL)
		{
			clean_up();
			return(*exit_code());
		}
		if(ft_isnum_exit(cmd_node->cmd[1]) == 0 &&  cmd_node->cmd[2] != NULL)
		{	
			clean_up();
			return(*exit_code());
		}
		clean_up();
		return (*exit_code());
	}
	else
		ft_putstr_fd("Command not found\n", 2);
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	reset_redirection(saved_stdin, saved_stdout);
	close(saved_stdin);
	close(saved_stdout);
	return (0);
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
