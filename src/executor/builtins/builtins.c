#include "minishell.h"

void	print_loop(t_cmd_node *cmd_node, int *i)
{
	while (cmd_node->cmd[*i])
	{
		ft_putstr_fd(cmd_node->cmd[*i], 1);
		if (cmd_node->cmd[*i + 1])
			ft_putstr_fd(" ", 1);
		(*i)++;
	}
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
		if (cmd_node->cmd[i] && cmd_node->cmd[i][0])
			print_loop(cmd_node, &i);
		if (no_flag)
			ft_putstr_fd("\n", 1);
	}
	else
		ft_putstr_fd("\n", 1);
	*exit_code() = 0;
}

int	count_argument(char **argument)
{
	int	count;

	count = 0;
	while (argument[count] != NULL)
		count++;
	return (count);
}

char	*get_home(t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("HOME", env_node->type) == 0)
			return (gc_strdup(env_node->value));
		env_node = env_node->next;
	}
	return (NULL);
}

char	*get_old_pwd(t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("OLDPWD", env_node->type) == 0)
			return (gc_strdup(env_node->value));
		env_node = env_node->next;
	}
	return (NULL);
}

void	update_old_pwd(char *old_pwd, t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("OLDPWD", env_node->type) == 0)
		{
			env_node->value = gc_strdup(old_pwd);
			return ;
		}
		env_node = env_node->next;
	}
}

void	update_new_pwd(char *new_pwd, t_env_list *env_list)
{
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("PWD", env_node->type) == 0)
		{
			env_node->value = new_pwd;
			return ;
		}
		env_node = env_node->next;
	}
}

void	builtin_cd(t_cmd_node *cmd_node, t_env_list *env_list)
{
	char	*directory_name;
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	gc_add(old_pwd);
	if (cmd_node->cmd[1] == NULL || ft_strcmp(cmd_node->cmd[1], "~") == 0)
		directory_name = get_home(env_list);
	else if (ft_strcmp(cmd_node->cmd[1], "-") == 0)
		directory_name = get_old_pwd(env_list);
	else
		directory_name = cmd_node->cmd[1];
	if (directory_name == NULL)
	{
		*exit_code() = 1;
		ft_putstr_fd("HOME or OLDPWD not set\n", 2);
		return ;
	}
	if (chdir(directory_name) != 0)
	{
		*exit_code() = 1;
		ft_putstr_fd("No such a file or directory\n", 2);
		return ;
	}
	new_pwd = getcwd(NULL, 0);
	gc_add(new_pwd);
	update_old_pwd(old_pwd, env_list);
	update_new_pwd(new_pwd, env_list);
	if (cmd_node->cmd[1] && ft_strcmp(cmd_node->cmd[1], "-") == 0)
	{
		ft_putstr_fd(new_pwd, 1);
		ft_putstr_fd("\n", 1);
	}
	*exit_code() = 0;
}

void	builtin_pwd(t_cmd_node *cmd_node)
{
	char	*pwd;

	(void)cmd_node;
	pwd = getcwd(NULL, 0);
	if (pwd != NULL)
	{
		ft_putstr_fd(pwd, 1);
		free(pwd);
		ft_putstr_fd("\n", 1);
	}
	else
		ft_putstr_fd("get_cwd function failed (returned NULL)\n", 2);
	*exit_code() = 0;
}

int	ft_isnum(char *str)
{
	int count;

	count = 0;
	while(str[count] != '\0')
	{
		if(!(str[count] >= 48 && str[count] <= 57))
		{
			ft_putstr_fd("numeric argument required\n", 2);
			return(-1);
		}
		count++;
	}
	return (0);
}

void builtin_exit(t_cmd_node *cmd_node)
{
	int		exit_val;

	if (cmd_node->cmd[0] || (cmd_node->cmd[0] && cmd_node->cmd[1]))
	{
		if (cmd_node->cmd[0] && cmd_node->cmd[1])
		{
			if(ft_isnum(cmd_node->cmd[1]) == -1)
			{
				*exit_code() = 255;
				return ;
			}
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
