#include "executor.h"
#include "minishell.h"

void execute(t_env_list *env_list, t_cmd_list *cmd_list)
{
	// DEBUG_INFO("starting execution\n");

	// if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
	// 	single_builtin_execution(cmd_list); // single builtin
	// else
	if (!cmd_list->syntax_error)
		execution(cmd_list, env_list); // normal execution
	return ;
}

void	single_builtin_execution(t_cmd_list *cmd_list)
{
	if(strcmp(cmd_list->head->cmd[0], "exit") == 0)
		exit(0);
}

void	execution(t_cmd_list *cmd_list, t_env_list *env_list)
{
	int saved_stdin;
	int saved_stdout;

	saved_stdout = 0;
	saved_stdin = 0;
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	execution_loop(cmd_list, env_list);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
{
	pid_t pid;	// speichert die process id von dem jeweiligen prozzess (child und parent)
	t_cmd_node *cmd_node;			// momentane cmd_node die ausgefürt wird
	int pipe_fd[2];
	int prev_pipe_fd[2] = {-1, -1};
	// ssize_t count;
	int		status;
	int waited;
	waited = 0;
	// count = 0;
	cmd_node = cmd_list->head;		// setzt den potiner von der metastruct auf die cmd_node damit der pointer von dem metastruct nicht verschoben werde muss
	while(cmd_node)
	{
		// ft_putstr_fd("We are in the loop\n", 2);
		if(cmd_node->next) {
			pipe_creation(pipe_fd);
		}
		pid =  fork();
		if(pid < 0)
			return (ft_putstr_fd("\033Error  forking", 2));
		else if(pid == 0)
		{
		if (prev_pipe_fd[0] != -1)
            dup2(prev_pipe_fd[0], STDIN_FILENO);
        if (cmd_node->next)
            dup2(pipe_fd[1], STDOUT_FILENO);
        // if (pipe_fd[0] != -1)
        //     close(pipe_fd[0]);
        // if (pipe_fd[1] != -1)
        //     close(pipe_fd[1]);
        if (prev_pipe_fd[0] != -1)
            close(prev_pipe_fd[0]);
        if (prev_pipe_fd[1] != -1)
            close(prev_pipe_fd[1]);
			// DEBUG_INFO("FROM LOOP-> HEAD ADRESS: %p \t NDOE ADRESS: %p", cmd_list->head, cmd_node);
			child_proccess(cmd_node, env_list);
		}
		if (prev_pipe_fd[0] != -1)
        	close(prev_pipe_fd[0]);
    	if (prev_pipe_fd[1] != -1)
        	close(prev_pipe_fd[1]);
    	prev_pipe_fd[0] = pipe_fd[0];
    	prev_pipe_fd[1] = pipe_fd[1];
		cmd_node = cmd_node->next;
	}
	// close_pipes(pipe_fd);
	while((waited = waitpid(-1, &status, 0)) > 0)
		;
		// ft_putstr_fd("waiting\n", 2);
}

int is_fd_open(int fd)
{
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}
void    check_fds(void)
{
    int check = 0;
    for(int i = 0; i < 21; i++)
    {
        check = is_fd_open(i);
        if (i < 3)
        {
            ft_putstr_fd("\033[33mfd ", 2);
            ft_putstr_fd(ft_itoa(i), 2);
            ft_putstr_fd(" is: \033[0m", 2);
        }
        else
        {
            ft_putstr_fd("fd ", 2);
            ft_putstr_fd(ft_itoa(i), 2);
            ft_putstr_fd(" is: ", 2);
        }
        if (check == 1)
            ft_putstr_fd("\033[32mopen\033[0m\n", 2);
        else
            ft_putstr_fd("\033[31mclosed\033[0m\n", 2);
    }
}