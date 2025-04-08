#ifndef EXECUTOR_H
# define EXECUTOR_H

//Includes -- BEGIN
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "minishell.h"
//Includes -- END

typedef struct s_cmd_list t_cmd_list;


//Functions -- BEGIN

// execute.c
void	execute(char **env);
void	single_builtin_execution(t_cmd_list *cmd_list);
void	single_cmd_execution(t_cmd_list *cmd_list, char **env);
void	child_proccess(t_cmd_node *cmd_node, int *pipe_fd, char **env);
void	parent_proccess(t_cmd_node *cmd_node, int *pipe_fd);


// io_redir.c 
int	redir_infile(t_file_node *file_node, int *pipe_fd);
int	redir_here_doc(t_file_node *file_node, int *pipe_fd);
int create_here_doc(t_file_node *file_node);
int	redir_outfile(t_file_node *file_node, int *pipe_fd);
int	redir_append(t_file_node *file_node, int *pipe_fd);

// io_redir_utils.c
int file_redirecting_child(t_cmd_node *cmd_node, int *pipe_fd);
int file_redirecting_parent(t_cmd_node *cmd_node, int *pipe_fd);
int	redir_loop(t_cmd_node *cmd_node,int *pipe_fd);

// process_utils.c
void reset_redirection(int saved_stdin, int saved_stdout);
void save_stdin_stdout(int *saved_stdin, int *saved_stdout);
void	fd_error_handle(int *fd);


// testing.c
void 	free_structs(t_cmd_list *cmd_list);
void	init_structs(t_cmd_list **cmd_list);
void	fill_structs(t_cmd_list *cmd_list);	
void	print_structs(t_cmd_list *cmd_list);


//Functions -- END

typedef enum OUTFILE_CHECK				// got the redirecting types for the files
{
	OUTFILE_NOT_USED,
	OUTFILE_USED
}	OUTFILE_CHECK;

typedef enum ERRORS				// got the redirecting types for the files
{
	ERROR = -1
}	ERRORS;
#endif
