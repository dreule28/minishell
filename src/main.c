#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	// char	*prompt;
	// int		exit_code;
	(void)argc;
	(void)argv;
	(void)env;
	// (void)exit_code;
	// check_fds();
	// ft_putstr_fd("BEFORE INIT\n", 2);
	gc_init();
	// while (1)
	// {
	// 	ft_putstr_fd("We are here\n", 2);
	// 	prompt = readline("2, 3 Years dagestan forget > ");
	// 	if (prompt)
	// 	{
	// 		gc_add(prompt); 								// adds the prompt string to the garbage collector
	// 		// fromp_parser(prompt);
			execute(env);
	// 		history_add("");
	// 		if (ft_strncmp(prompt, "exit", 4) == 0)
	// 			break ;
	// 	}
	// 	clean_up(); 
	// }
	// return (0);
}

// #include <errno.h>
// void execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
// {
// 	pid_t pid;	// speichert die process id von dem jeweiligen prozzess (child und parent)
// 	t_cmd_node *cmd_node;			// momentane cmd_node die ausgefürt wird
// 	int pipe_fd[2];
// 	ssize_t count;
// 	int		status;
	
// 	count = 0;
// 	cmd_node = cmd_list->head;		// setzt den potiner von der metastruct auf die cmd_node damit der pointer von dem metastruct nicht verschoben werde muss
// 	while(cmd_node)
// 	{
// 		ft_putstr_fd("We are in the loop\n", 2);
// 		if(cmd_node->next != NULL)
// 			pipe_creation(pipe_fd);
// 		pid =  fork();
// 		if(pid < 0)		
// 			return (ft_putstr_fd("Error  forking", 2));		
// 		if(pid == 0)
// 			child_proccess(cmd_node, pipe_fd, env_list);
// 		if(cmd_node->next == NULL)
// 			break ;
// 		cmd_node = cmd_node->next;
// 	}
// 	while(count <= cmd_list->size )
// 	{
// 		count++;
// 		waitpid(-1, &status, 0);
// 	}
// 	close_pipes(pipe_fd);
// }