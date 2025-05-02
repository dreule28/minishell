#include "minishell.h"

int	execute(t_env_list *env_list, t_cmd_list *cmd_list)
{
	int	check;

	check = 0;
	if (!cmd_list->head)
		if (cmd_list->head->cmd[0] && cmd_list->head->cmd[1]
			&& cmd_list->head->cmd_type == BUILTIN)
		{
			if (!ft_strncmp(cmd_list->head->cmd[0], "exit",
					ft_strlen(cmd_list->head->cmd[0]))
				&& cmd_list->head->cmd[1] == NULL)
			{
				return (0);
			}
		}
	if (cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
	{
		check = single_builtin_execution(cmd_list->head, env_list);
		if (check != 0)
			return (0);
		else
			return (1);
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
		else
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

	while ((waited = waitpid(-1, &status, 0)) > 0)
	{
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
		pid_t last_pid)
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
		last_pid = pid;
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
		if (execute_cmd_loop(cmd_node, env_list, last_pid))
			return ;
		cmd_node = cmd_node->next;
	}
	wait_for_child(last_pid, old_int);
}
