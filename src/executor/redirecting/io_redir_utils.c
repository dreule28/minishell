#include "minishell.h"

int	file_redirecting(t_cmd_node *cmd_node, t_env_list *env_list)
{
	if (redir_loop_infiles(cmd_node, env_list) == -1)
		return (-1);
	if (redir_loop_outfiles(cmd_node) == -1)
		return (-1);
	return (0);
}

void	delete_tmp_files(const char *folder_name)
{
	DIR				*dir;
	char			*file_name;
	char			*tmp_str;
	struct dirent	*file;

	dir = opendir(folder_name);
	while ((file = readdir(dir)) != NULL)
	{
		if (file->d_name[0] == '.')
		{
			tmp_str = ft_strjoin(folder_name, "/");
			file_name = ft_strjoin(tmp_str, file->d_name);
			free(tmp_str);
			unlink(file_name);
			free(file_name);
		}
	}
	closedir(dir);
}

void	set_interaktive_line(void)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd == -1)
	{
		ft_putstr_fd("Could not open /dev/tty\n", 2);
		return ;
	}
	dup2(tty_fd, STDIN_FILENO);
	dup2(tty_fd, STDOUT_FILENO);
	close(tty_fd);
}

void	write_here_doc_file_expand(char *line, int write_fd, t_env_list *env_list)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), 1);
	token->value = gc_strdup(line);
	token->segment_list = NULL;
	split_token_into_heredocs(token);
	expand_token(token, env_list);
	ft_putstr_fd(token->value, write_fd);
	ft_putstr_fd("\n", write_fd);
}
