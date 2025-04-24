#include "minishell.h"

void	builtin_echo(t_cmd_node *cmd_node, t_env_list *env_list)
{
	(void)cmd_node;
	(void)env_list;
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
	char		*home_path;
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("HOME", env_node->type) == 0)
		{
			home_path = ft_malloc(ft_strlen(env_node->type), sizeof(char));
			home_path = env_node->value;
			return (home_path);
		}
		env_node = env_node->next;
	}
	return (NULL);
}

char	*get_old_pwd(t_env_list *env_list)
{
	char		*old_pwd_path;
	t_env_node	*env_node;

	env_node = env_list->head;
	while (env_node)
	{
		if (ft_strcmp("OLDPWD", env_node->type) == 0)
		{
			old_pwd_path = ft_malloc(ft_strlen(env_node->type), sizeof(char));
			old_pwd_path = env_node->value;
			return (old_pwd_path);
		}
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
			env_node->value = old_pwd;
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
		ft_putstr_fd("HOME or OLDPWD not set\n", 2);
		free(old_pwd);
		return ;
	}
	if (chdir(directory_name) != 0)
	{
		ft_putstr_fd("No such a file or directory\n", 2);
		free(old_pwd);
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
}

void	builtin_exit(t_cmd_node *cmd_node, t_env_list *env_list)
{
	(void)cmd_node;
	(void)env_list;
}
