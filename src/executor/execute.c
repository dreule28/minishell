/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gzovkic <gzovkic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:51:30 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/02 17:51:31 by gzovkic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd_or_builtin(t_cmd_node *cmd_node, t_env_list *env_list)
{
	int	exit_status;

	if (cmd_node->cmd_type == BUILTIN)
		exit_status = single_builtin_execution(cmd_node, env_list);
	else
		exit_status = child_proccess(cmd_node, env_list);
	clean_up();
	exit(exit_status);
}

bool	execute_cmd_loop(t_cmd_node *cmd_node, t_env_list *env_list,
		pid_t *last_pid)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		prev_pipe_fd[2];

	pid = 0;
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	prev_pipe_fd[0] = -1;
	prev_pipe_fd[1] = -1;
	if (set_up_pipe(cmd_node, pipe_fd, &pid))
		return (true);
	if (pid == 0)
	{
		if (set_pipes_child(cmd_node, pipe_fd, prev_pipe_fd) == -1)
			return (true);
		execute_cmd_or_builtin(cmd_node, env_list);
	}
	if (cmd_node->next == NULL)
		*last_pid = pid;
	set_pipes_parent(pipe_fd, prev_pipe_fd);
	return (false);
}

void	execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
{
	t_cmd_node			*cmd_node;
	pid_t				last_pid;
	struct sigaction	old_int;
	struct sigaction	ignore;

	last_pid = -1;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	sigaction(SIGINT, &ignore, &old_int);
	g_sigint_status = 3;
	cmd_node = cmd_list->head;
	if (here_doc_creation(cmd_node, env_list))
		return ;
	cmd_node = cmd_list->head;
	while (cmd_node)
	{
		if (execute_cmd_loop(cmd_node, env_list, &last_pid))
			return ;
		cmd_node = cmd_node->next;
	}
	wait_for_child(last_pid, old_int);
}
