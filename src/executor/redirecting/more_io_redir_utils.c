/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_io_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:51:39 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 17:51:40 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*convert_file_name(char *file_name, int i)
{
	int		count;
	char	*converted;
	char	*numbered_str;

	count = 0;
	converted = gc_strdup(file_name);
	while (converted[count] != '\0')
	{
		if (!(ft_isalnum(converted[count])))
			converted[count] = '_';
		count++;
	}
	numbered_str = gc_strjoin(converted, gc_itoa(i));
	return (numbered_str);
}

int	is_interactive_shell(void)
{
	return (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO));
}

bool	here_doc_loop(t_file_node *file_node, t_env_list *env_list,
		int write_fd)
{
	char	*line;

	if (is_interactive_shell())
		line = readline("> ");
	else
		line = get_next_line(STDIN_FILENO);
	if (!line || g_sigint_status == 2)
	{
		if (g_sigint_status == 2)
		{
			start_signals();
			close(write_fd);
			return (NULL);
		}
		return (true);
	}
	if (ft_strcmp(line, file_node->filename) == 0)
	{
		free(line);
		return (true);
	}
	write_here_doc(line, write_fd, env_list, file_node);
	free(line);
	return (false);
}
