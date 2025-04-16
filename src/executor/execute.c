#include "executor.h"
#include "minishell.h"

void execute(char **env)
{
	t_env_list *env_list;
	
	t_cmd_list *cmd_list;
	cmd_list = init_cmd_list();

	t_cmd_node *cmd_node1; // declare a pointer to a struct of type t_cmd_node1
	char *cmd1[] = {"cat", NULL};
	cmd_node1 = add_cmd_node(cmd_list,cmd1, CMD); // add a node to the struct
	add_file_node(cmd_node1->files, "END1", HEREDOC);

	
	// t_cmd_node *cmd_node2; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd2[] = {"echo", "second" , NULL};
	// cmd_node2 = add_cmd_node(cmd_list,cmd2, CMD); // add a node to the struct
	// add_file_node(cmd_node2->files, "file.txt", OUTFILE_APPEND);

	// t_cmd_node *cmd_node3; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd3[] = {"cat" , NULL};
	// cmd_node3 = add_cmd_node(cmd_list,cmd3, CMD); // add a node to the struct
	// add_file_node(cmd_node3->files, "outfile2", INFILE);

	// t_cmd_node *cmd_node4; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd4[] = {"cat"  , NULL};
	// cmd_node4 = add_cmd_node(cmd_list,cmd4, CMD); // add a node to the struct
	// add_file_node(cmd_node4->files, "file.txt", INFILE);

	// t_cmd_node *cmd_node5; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd5[] = {"cat" , NULL};
	// cmd_node5 = add_cmd_node(cmd_list,cmd5, CMD); // add a node to the struct
	// add_file_node(cmd_node5->files, "infile1", INFILE);

	env_list = get_envs(env);

	if(cmd_list->size == 1 && cmd_list->head->cmd_type == BUILTIN)
		single_builtin_execution(cmd_list); // single builtin
	else
		execution(cmd_list, env_list); // normal execution
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
	// check_fds();
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	execution_loop(cmd_list, env_list);
	ft_putstr_fd("We are after execution loop\n", 2);
	reset_redirection(saved_stdin, saved_stdout);
	close(saved_stdin);
	close(saved_stdout);
	ft_putstr_fd("WE ARE IN EXEC\n", 2);
	check_fds();
}

#include <errno.h>
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
        if (pipe_fd[0] != -1)
            close(pipe_fd[0]);
        if (pipe_fd[1] != -1)
            close(pipe_fd[1]);
        if (prev_pipe_fd[0] != -1)
            close(prev_pipe_fd[0]);
        if (prev_pipe_fd[1] != -1)
            close(prev_pipe_fd[1]);
			// DEBUG_INFO("FROM LOOP-> HEAD ADRESS: %p \t NDOE ADRESS: %p", cmd_list->head, cmd_node);
			child_proccess(cmd_node, pipe_fd, env_list , cmd_list);
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
		ft_putstr_fd("waiting\n", 2);
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