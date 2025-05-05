/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redir_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:51:44 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/05 17:31:47 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_redirecting(t_cmd_node *cmd_node)
{
	if (redir_loop_infiles(cmd_node) == -1)
		return (-1);
	if (redir_loop_outfiles(cmd_node) == -1)
		return (-1);
	return (0);
}

void	delete_tmp_files(const char *folder_name)
{
	DIR				*dir;
	struct dirent	*file;
	char			*tmp_str;
	char			*file_name;

	dir = opendir(folder_name);
	if (!dir)
		return ;
	file = readdir(dir);
	while (file != NULL)
	{
		if (ft_strncmp(file->d_name, ".here_doc", 9) == 0)
		{
			tmp_str = ft_strjoin(folder_name, "/");
			file_name = ft_strjoin(tmp_str, file->d_name);
			free(tmp_str);
			unlink(file_name);
			free(file_name);
		}
		file = readdir(dir);
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

void	write_here_doc_file_expand(char *line, int write_fd,
		t_env_list *env_list)
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
