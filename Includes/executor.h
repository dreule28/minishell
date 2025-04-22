#ifndef EXECUTOR_H
# define EXECUTOR_H

// Includes -- BEGIN
# include "minishell.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
// Includes -- END

typedef struct s_cmd_list	t_cmd_list;

// Functions -- BEGIN

// execute.c
void						execute(t_env_list *env_list, t_cmd_list *cmd_list);
void						execution(t_cmd_list *cmd_list,
								t_env_list *env_list);
void						execution_loop(t_cmd_list *cmd_list,
								t_env_list *env_list);

// debug_utils.c
int							is_fd_open(int fd);
void						check_fds(void);

// redirecting/io_redir_utils.c
int							file_redirecting(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						delete_tmp_files(const char *folder_name);
void						set_interaktive_line(void);
void						write_here_doc_file_expand(char *line, int write_fd,
								t_env_list *env_list);

// redirecting/io_redir.c
int							redir_infile(t_file_node *file_node);
int							redir_here_doc(t_file_node *file_node,
								t_env_list *env_list);
char						*create_here_doc(t_file_node *file_node,
								t_env_list *env_list);
int							redir_outfile(t_file_node *file_node);
int							redir_append(t_file_node *file_node);

// redirecting/io_redir_loops.c
int							redir_loop_infiles(t_cmd_node *cmd_node,
								t_env_list *env_list);
int							redir_loop_outfiles(t_cmd_node *cmd_node);
int	create_here_doc_files(t_cmd_node *cmd_node, t_env_list *env_list);


// redirecion/here_doc_utils.c
int count_single_quotes(char *line);
int count_double_quotes(char *line);
void 	write_here_doc_file(char *line, int write_fd);
void	write_here_doc(char *line, int write_fd, t_env_list *env_list, t_file_node *file_node);


// process/env_functions.c
int							env_lenght(t_env_list *env_list);
char						**env_converter(t_env_list *env_list);
char						**env_converter_export(t_env_list *env_list);
char						*env_search_path_var(t_env_list *env_list);

// process/process_utils.c
void						reset_redirection(int saved_stdin,
								int saved_stdout);
void						save_stdin_stdout(int *saved_stdin,
								int *saved_stdout);
void						pipe_creation(int *fd);
int							error_check_null(char *str);

// process/pipe_utils.c
void						close_pipes(int *pipe_fd);
void						set_invalid_pipe(int *prev_pipe_fd);
void						set_new_prev_pipe(int *prev_pipe_fd, int *pipe_fd);
int							set_pipes_child(t_cmd_node *cmd_node, int *pipe_fd,
								int *prev_pipe_fd);
void						set_pipes_parent(int *pipe_fd, int *prev_pipe_fd);

// process/processes.c
void						child_proccess(t_cmd_node *cmd_node,
								t_env_list *env_list);
char						*create_command_path(t_cmd_node *cmd_node,
								char **env_path_list);
void						execute_command(t_cmd_node *cmd_node,
								char **env_path_list, t_env_list *env_list);

// builtins/utils.c
void						single_builtin_execution(t_cmd_node *cmd_node,
								t_env_list *env_list);
void	sort_env_array(char **converted_env_list);


// builtins/builtins.c
void						builtin_echo(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						builtin_cd(t_cmd_node *cmd_node);
void						builtin_pwd(t_cmd_node *cmd_node);
void						builtin_exit(t_cmd_node *cmd_node, t_env_list *env_list);

// builtins/env_builtins.c
void						builtin_env(t_env_list *env_list);
void						builtin_export(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						builtin_unset(t_cmd_node *cmd_node,
								t_env_list *env_list);

// builtins/env_builtins_utils.c
char *get_type(char *str);
char *get_value(char *str);
int	check_duplicates(t_cmd_node *cmd_node, t_env_list *env_list);
void export_variable(t_cmd_node *cmd_node, t_env_list *env_list);
void	print_export_variables(t_env_list *env_list);



// Functions -- END

typedef enum FILE_CHECK // got the redirecting types for the files
{
	OUTFILE_NOT_USED,
	OUTFILE_USED,
	INFILE_NOT_USED,
	INFILE_USED
}							FILE_CHECK;

typedef enum ERRORS // got the redirecting types for the files
{
	ERROR = -1
}							ERRORS;
#endif
