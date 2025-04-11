#include "executor.h"

void execute(char **env)
{
	t_env_list *env_list;
	
	t_cmd_list *cmd_list;
	cmd_list = init_cmd_list();

	t_cmd_node *cmd_node1; // declare a pointer to a struct of type t_cmd_node1
	char *cmd1[] = {"cat", NULL};
	cmd_node1 = add_cmd_node(cmd_list,cmd1, CMD); // add a node to the struct
	add_file_node(cmd_node1->files, "infile1", INFILE);

	t_cmd_node *cmd_node2; // declare a pointer to a struct of type t_cmd_node1
	char *cmd2[] = {"cat", NULL};
	cmd_node2 = add_cmd_node(cmd_list,cmd2, CMD); // add a node to the struct
	add_file_node(cmd_node2->files, "outfile1", OUTFILE_APPEND);

	// t_cmd_node *cmd_node3; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd3[] = {"ls", NULL};
	// cmd_node3 = add_cmd_node(cmd_list,cmd3, CMD); // add a node to the struct
	// add_file_node(cmd_node3->files, "outfil123123e3", OUTFILE_APPEND);

	// t_cmd_node *cmd_node4; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd4[] = {"ls", NULL};
	// cmd_node4 = add_cmd_node(cmd_list,cmd4, CMD); // add a node to the struct
	// add_file_node(cmd_node4->files, "outfile4", OUTFILE_APPEND);

	// t_cmd_node *cmd_node5; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd5[] = {"ls", NULL};
	// cmd_node5 = add_cmd_node(cmd_list,cmd5, CMD); // add a node to the struct
	// add_file_node(cmd_node5->files, "outfile5", OUTFILE_APPEND);

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
	save_stdin_stdout(&saved_stdin, &saved_stdout);
	execution_loop(cmd_list, env_list);
	reset_redirection(saved_stdin, saved_stdout);
	ft_putstr_fd("ende\n \n \n ", 2);
	close(saved_stdin);
	close(saved_stdout);
}

void execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
{
	pid_t pid;	// speichert die process id von dem jeweiligen prozzess (child und parent)
	t_cmd_node *cmd_node;			// momentane cmd_node die ausgefürt wird
	int pipe_fd[2];
	
	cmd_node = cmd_list->head;		// setzt den potiner von der metastruct auf die cmd_node damit der pointer von dem metastruct nicht verschoben werde muss
	while(cmd_node)
	{

		if(cmd_node->next != NULL)
			pipe_creation(pipe_fd);
		else
		{
			// pipe_creation(pipe_fd);
			dup2(pipe_fd[1], STDOUT_FILENO);
		}
			pid =  fork();
		if(pid < 0)		
			return (ft_putstr_fd("Error  forking", 2));		
		if(pid == 0)
		{
			child_proccess(cmd_node, pipe_fd, env_list);
			ft_putstr_fd("child hat nicht richtif fetig gemacht \n \n \n \n \n \n \n \n \n \n \n \n \n \n\n \n \n \n ", 2);
		}
		else
		{
			waitpid(pid, NULL, 0);
			ft_putstr_fd("ende\n \n \n ", 2);
			// if(cmd_node->next != NULL)
			// {
			// 	close(pipe_fd[0]);
			// 	close(pipe_fd[1]);
			// }
		}

		cmd_node = cmd_node->next;
	}
	ft_putstr_fd("ende\n \n \n ", 2);

}
