/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:51:41 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 17:51:42 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_infile(t_file_node *file_node)
{
	int	file_redirecting;

	if (access(file_node->filename, F_OK) == -1)
	{
		ft_putstr_fd("No such file or directory\n", 2);
		return (-1);
	}
	file_redirecting = open(file_node->filename, O_RDONLY);
	if (file_redirecting == -1)
		return (-1);
	if (dup2(file_redirecting, STDIN_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2(infile)stdin\n", 2), -1);
	close(file_redirecting);
	return (INFILE_USED);
}

int	redir_here_doc(t_file_node *file_node, t_env_list *env_list, int i)
{
	int		file_redirecting;
	char	*file_name;
	char	*converted_file_name;

	(void)env_list;
	file_redirecting = 0;
	converted_file_name = convert_file_name(file_node->filename, i);
	(i)++;
	file_name = gc_strjoin("tmp/.here_doc_", converted_file_name);
	if (g_sigint_status == 2)
		return (-1);
	file_redirecting = open(file_name, O_RDONLY);
	if (file_redirecting == -1)
	{
		ft_putstr_fd("Operation not permitted\n", 2);
		return (-1);
	}
	if (dup2(file_redirecting, STDIN_FILENO) == -1)
	{
		ft_putstr_fd("Error using dup2(here_doc)stdin\n", 2);
		return (-1);
	}
	close(file_redirecting);
	return (INFILE_USED);
}

char	*create_here_doc(t_file_node *file_node, t_env_list *env_list, int i)
{
	int		write_fd;
	char	*str;
	char	*converted_file_name;

	if (is_interactive_shell())
	{
		set_interaktive_line();
		start_heredoc_signals();
	}
	converted_file_name = convert_file_name(file_node->filename, i);
	str = gc_strjoin("tmp/.here_doc_", converted_file_name);
	write_fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (write_fd == -1)
		return (ft_putstr_fd("Operation not permitted\n", 2), NULL);
	while (1)
	{
		if (here_doc_loop(file_node, env_list, write_fd))
			return (NULL);
	}
	close(write_fd);
	if (is_interactive_shell())
		start_signals();
	return (str);
}

int	redir_outfile(t_file_node *file_node)
{
	int	file_redirecting;

	file_redirecting = open(file_node->filename, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (file_redirecting == -1)
	{
		ft_putstr_fd("Operation not permitted\n", 2);
		return (-1);
	}
	if (dup2(file_redirecting, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2(outfile)stdout\n", 2), -1);
	close(file_redirecting);
	return (OUTFILE_USED);
}

int	redir_append(t_file_node *file_node)
{
	int	file_redirecting;

	file_redirecting = open(file_node->filename, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (file_redirecting == -1)
	{
		ft_putstr_fd("Operation not permitted\n", 2);
		return (-1);
	}
	if (dup2(file_redirecting, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error using dup2(append)stdout\n", 2), -1);
	close(file_redirecting);
	return (OUTFILE_USED);
}
