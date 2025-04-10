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

	// t_cmd_node *cmd_node2; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd2[] = {"cat", NULL};
	// cmd_node2 = add_cmd_node(cmd_list,cmd2, CMD); // add a node to the struct
	// add_file_node(cmd_node2->files, "outfile2", OUTFILE);

	// t_cmd_node *cmd_node3; // declare a pointer to a struct of type t_cmd_node1
	// char *cmd3[] = {"ls", NULL};
	// cmd_node3 = add_cmd_node(cmd_list,cmd3, CMD); // add a node to the struct
	// add_file_node(cmd_node3->files, "outfile3", OUTFILE_APPEND);

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
}

// void execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
// {
// 	pid_t pid;
// 	int pipe_fd[2];
// 	int prev_pipe_fd[2];
// 	t_cmd_node	*cmd_node;

// 	cmd_node = cmd_list->head;
// 	set_invalid_pipe(prev_pipe_fd);
// 	while(cmd_node)
// 	{
// 		if(cmd_node->next)
// 			pipe_creation(pipe_fd);
// 		else
// 			dup_stdin_stdout(pipe_fd);
// 		pid = fork();
// 		if(pid < 0)
// 			return (ft_putstr_fd("Error while forking", 2));
// 		if(pid == 0)
// 		{
// 			if(prev_pipe_fd[0] != -1)
// 				dup2(prev_pipe_fd[0], STDIN_FILENO);
// 			// if(cmd_node->next)
// 			// 	dup2(pipe_fd[1], STDOUT_FILENO);
// 			if(prev_pipe_fd[0] == -1)
// 				close_pipes(prev_pipe_fd);
// 			// if(cmd_node->next)
// 			// 	close_pipes(pipe_fd);
// 			child_proccess(cmd_node, pipe_fd, env_list);
// 		}
// 		else
// 		{
// 			waitpid(pid, NULL, 0);
// 			if(prev_pipe_fd[0] == -1)
// 				close_pipes(prev_pipe_fd);
// 			if(cmd_node->next)	
// 				set_new_prev_pipe(pipe_fd, prev_pipe_fd);
// 		}
// 		cmd_node = cmd_node->next;
// 	}
// 	if(prev_pipe_fd[0] == -1)
// 		close_pipes(prev_pipe_fd);
// }

void execution_loop(t_cmd_list *cmd_list, t_env_list *env_list)
{
	pid_t pid;						// speichert die process id von dem jeweiligen prozzess (child und parent)
	int pipe_fd[2];					// der file descriptor der bestimmt ob in stdin oder stdout geschrieben wird
	int prev_pipe_fd[2];			// ??
	t_cmd_node *cmd_node;			// momentane cmd_node die ausgefürt wird

	cmd_node = cmd_list->head;		// setzt den potiner von der metastruct auf die cmd_node damit der pointer von dem metastruct nicht verschoben werde muss
	set_invalid_pipe(prev_pipe_fd);	// macht die prev_pipe invalid (setzt auf -1) damit der childproccess beim ersten mal den stdin benutzt
	while(cmd_node)					// solange eine cmd_node exestiert hält der loop an
	{
		if(cmd_node->next)			// wenn noch ein command folgt erstelle eine pipe 
			pipe_creation(pipe_fd);	// erstellt eine pipe
		else								// wenn nicht nehme die stdin und stdout als file descriptor
			dup_stdin_stdout(pipe_fd);		// gibt der pipe_fd stdin und stdout
		pid = fork();						// erstellt die processes
		if(pid < 0)							// wenn fork gefailt hat
			return (ft_putstr_fd("Error  forking", 2));		// error message
		if(pid == 0)										// wenn childproccess ist
		{
			(void)env_list;
			if(prev_pipe_fd[0] != -1)														// wenn eine valid file vorhanden ist redirected es diese vorhandene file als stdin
				if(dup2(prev_pipe_fd[0], STDIN_FILENO) < 0)
					ft_putstr_fd("Error duplicating previous pipe to STDIN\n", 2);
			if(cmd_node->next)															// wenn es eine nächste node gibt nimm diese als stdout(also scheibt es in die pipe)
				if(dup2(pipe_fd[1], STDOUT_FILENO) < 0)
					ft_putstr_fd("Error duplicating current pipe to STDOUT\n", 2);
			if (prev_pipe_fd[0] != -1)
				close_pipes(prev_pipe_fd);
			// if(cmd_node->next)
			// 	close_pipes(pipe_fd);
			child_proccess(cmd_node, pipe_fd, env_list);	// execute command
		}
		else												// wenn parent proccess
		{
			waitpid(pid, NULL, 0);							// warte auf child 
			// ft_putstr_fd("Parent: child finished, updating pipes\n", 2);
			if (prev_pipe_fd[0] != -1)
                close_pipes(prev_pipe_fd);
			if(cmd_node->next)
			{
                // Close the write end in the parent to signal EOF to the reading child.
                close(pipe_fd[1]);
                // Save the read end as the previous pipe for the next iteration.
                prev_pipe_fd[0] = pipe_fd[0];
                prev_pipe_fd[1] = -1; // mark the write end as closed here in the parent.
            }
		}
		cmd_node = cmd_node->next;							// nächst command node
	}
	if (prev_pipe_fd[0] != -1)
		close_pipes(prev_pipe_fd);
}











