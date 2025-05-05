/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:51:35 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/05 19:02:12 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_env_list *env_list, t_cmd_list *cmd_list)
{
	int	check;

	check = 0;
	if (here_doc_creation(cmd_list->head, env_list) || !cmd_list->head)
		return (0);
	if (cmd_list->head->cmd && cmd_list->head->cmd[0] != NULL)
	{
		if (cmd_list->head->cmd[0] && cmd_list->head->cmd[1]
			&& cmd_list->head->cmd_type == BUILTIN)
		{
			if (!ft_strncmp(cmd_list->head->cmd[0], "exit",
					ft_strlen(cmd_list->head->cmd[0]))
				&& cmd_list->head->cmd[1] == NULL)
				return (0);
		}
	}
	if (cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
	{
		check = single_builtin_execution(cmd_list->head, env_list);
		if (check != 0)
			return (0);
	}
	else
		execution(cmd_list, env_list);
	return (1);
}

void	execution(t_cmd_list *cmd_list, t_env_list *env_list)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdout = 0;
	saved_stdin = 0;
	execution_loop(cmd_list, env_list);
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	reset_redirection(saved_stdin, saved_stdout);
	close(saved_stdin);
	close(saved_stdout);
}

bool	here_doc_creation(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int	count_here_doc;
	int	here_doc_nbr;

	count_here_doc = 1;
	while (cmd_node)
	{
		here_doc_nbr = create_here_doc_files(cmd_node, env_list,
				count_here_doc);
		if (here_doc_nbr == -1)
			return (true);
		count_here_doc = here_doc_nbr;
		cmd_node = cmd_node->next;
	}
	return (false);
}

void	wait_for_child(pid_t last_pid, struct sigaction old_int)
{
	int	status;
	int	last_status;
	int	waited;

	status = 0;
	last_status = 0;
	waited = 0;
	while (1)
	{
		waited = waitpid(-1, &status, 0);
		if (waited <= 0)
			break ;
		if (waited == last_pid)
			last_status = status;
	}
	sigaction(SIGINT, &old_int, NULL);
	g_sigint_status = 0;
	if (WIFSIGNALED(last_status))
		*exit_code() = 128 + WTERMSIG(last_status);
	else
		*exit_code() = WEXITSTATUS(last_status);
}

bool	set_up_pipe(t_cmd_node *cmd_node, int *pipe_fd, pid_t *pid)
{
	if (cmd_node->next)
		pipe_creation(pipe_fd);
	*pid = fork();
	if (*pid < 0)
	{
		ft_putstr_fd("Error  forking\n", 2);
		return (true);
	}
	return (false);
}
