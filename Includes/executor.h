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
void	 execute(t_env_list *env_list, t_cmd_list *cmd_list);
void	single_builtin_execution(t_cmd_list *cmd_list);
void	execution(t_cmd_list *cmd_list, t_env_list *env_list);
void	execution_loop(t_cmd_list *cmd_list, t_env_list *env_list);


void	single_cmd_execution(t_cmd_list *cmd_list, char **env); // removed

// io_redir.c
int	redir_infile(t_file_node *file_node, int *pipe_fd);
int	redir_here_doc(t_file_node *file_node, int *pipe_fd);
int create_here_doc(t_file_node *file_node);
int	redir_outfile(t_file_node *file_node, int *pipe_fd);
int	redir_append(t_file_node *file_node, int *pipe_fd);

// io_redir_utils.c
int file_redirecting_child(t_cmd_node *cmd_node, int *pipe_fd, t_cmd_list *cmd_list);
int file_redirecting_parent(t_cmd_node *cmd_node, int *pipe_fd);
int	redir_loop(t_cmd_node *cmd_node,int *pipe_fd);

// process_utils.c
void reset_redirection(int saved_stdin, int saved_stdout);
void save_stdin_stdout(int *saved_stdin, int *saved_stdout);
void	pipe_creation(int *fd);

// processes.c
void	child_proccess(t_cmd_node *cmd_node, int *pipe_fd, t_env_list *env_list, t_cmd_list *cmd_list);
void	parent_proccess(t_cmd_node *cmd_node, int *pipe_fd, t_env_list *env_list);
char	*create_command_path(t_cmd_node *cmd_node, char **env_path_list);
void	execute_command(t_cmd_node *cmd_node, char **env_path_list, t_env_list *env_list);

// env_functions.c
int		env_lenght(t_env_list *env_list);
char	**env_converter(t_env_list *env_list);
char	*env_search_path_var(t_env_list *env_list);

// pipe_utils.c
void	dup_stdin_stdout(int *pipe_fd);
void	close_pipes(int *pipe_fd);
void	set_invalid_pipe(int *prev_pipe_fd);
void	set_new_prev_pipe(int *prev_pipe_fd, int *pipe_fd);


int redir_loop_infiles(t_cmd_node *cmd_node, int *pipe_fd);
int	redir_loop_outfiles(t_cmd_node *cmd_node,int *pipe_fd);

//Functions -- END


int is_fd_open(int fd);
void    check_fds(void);


typedef enum FILE_CHECK				// got the redirecting types for the files
{
	OUTFILE_NOT_USED,
	OUTFILE_USED,
	INFILE_NOT_USED,
	INFILE_USED
}	FILE_CHECK;

typedef enum ERRORS				// got the redirecting types for the files
{
	ERROR = -1
}	ERRORS;
#endif
