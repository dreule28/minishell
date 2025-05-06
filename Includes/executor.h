/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:43:09 by gzovkic           #+#    #+#             */
/*   Updated: 2025/05/06 10:40:07 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// builtins/single_builtin_execution
int							check_type_name(char *str);
int							ft_isnum(char *str);
int							ft_isnum_exit(char *str);
int							search_return_code(t_cmd_node *cmd_node);
bool						search_builtin(t_cmd_node *cmd_node,
								t_env_list *env_list, char *str);
void						reset_redir(int saved_stdin, int saved_stdout);

int							check_exit_arguments(t_cmd_node *cmd_node);
void						print_error_message(char *cmd, char *message);
int							execute(t_env_list *env_list, t_cmd_list *cmd_list);
void						execution(t_cmd_list *cmd_list,
								t_env_list *env_list);

bool						here_doc_creation(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						wait_for_child(pid_t last_pid,
								struct sigaction old_int);
bool						set_up_pipe(t_cmd_node *cmd_node, int *pipe_fd,
								pid_t *pid);
char						*convert_file_name(char *file_name, int i);
int							is_interactive_shell(void);
bool						here_doc_loop(t_file_node *file_node,
								t_env_list *env_list, int write_fd);

// builtins/cd_utils.c
int							count_argument(char **argument);
char						*get_home(t_env_list *env_list);
char						*get_old_pwd(t_env_list *env_list);
void						update_old_pwd(char *old_pwd, t_env_list *env_list);
void						update_new_pwd(char *new_pwd, t_env_list *env_list);

int							check_dir_names(char *old_dir, char *home_dir,
								char *dir_name);
void						update_pwds(t_cmd_node *cmd_node,
								t_env_list *env_list, char *old_pwd);
int							check_first_char(char *str);
void						check_export_behavior(t_cmd_node *cmd_node,
								t_env_list *env_list, int count);

int							unset_loop(t_cmd_node *cmd_node,
								t_env_node **env_node, t_env_list *env_list,
								int count);
int							is_valid_identifier(char *str);

// execute.c
int							execute(t_env_list *env_list, t_cmd_list *cmd_list);
void						execution(t_cmd_list *cmd_list,
								t_env_list *env_list);
void						execution_loop(t_cmd_list *cmd_list,
								t_env_list *env_list);

// debug_utils.c
int							is_fd_open(int fd);
void						check_fds(void);

// redirecting/io_redir_utils.c
int							file_redirecting(t_cmd_node *cmd_node);
void						delete_tmp_files(const char *folder_name);
void						set_interaktive_line(void);
void						write_here_doc_file_expand(char *line, int write_fd,
								t_env_list *env_list);

// redirecting/io_redir.c
int							redir_infile(t_file_node *file_node);
int							redir_here_doc(t_file_node *file_node);
char						*create_here_doc(t_file_node *file_node,
								t_env_list *env_list, int count_here_doc);
int							redir_outfile(t_file_node *file_node);
int							redir_append(t_file_node *file_node);

// redirecting/io_redir_loops.c
int							redir_loop_infiles(t_cmd_node *cmd_node);
int							redir_loop_outfiles(t_cmd_node *cmd_node);
int							create_here_doc_files(t_cmd_node *cmd_node,
								t_env_list *env_list, int count_here_doc);

// redirecion/here_doc_utils.c
int							count_single_quotes(char *line);
int							count_double_quotes(char *line);
void						write_here_doc_file(char *line, int write_fd);
void						write_here_doc(char *line, int write_fd,
								t_env_list *env_list, t_file_node *file_node);

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
int							child_proccess(t_cmd_node *cmd_node,
								t_env_list *env_list);
char						*create_command_path(t_cmd_node *cmd_node,
								char **env_path_list);
int							execute_command(t_cmd_node *cmd_node,
								char **env_path_list, t_env_list *env_list);

// builtins/utils.c
int							single_builtin_execution(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						sort_env_array(char **converted_env_list);
bool						is_all_flag(char *flag, char c);

// builtins/builtins.c
void						builtin_echo(t_cmd_node *cmd_node);
void						builtin_cd(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						builtin_pwd(void);
int							builtin_exit(t_cmd_node *cmd_node);

// builtins/env_builtins.c
void						builtin_env(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						builtin_export(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						builtin_unset(t_cmd_node *cmd_node,
								t_env_list *env_list);

// builtins/env_builtins_utils.c
void						export_variable(t_cmd_node *cmd_node,
								t_env_list *env_list);
void						print_export_variables(t_env_list *env_list);

// buitlins/export_utils.c
char						*get_type(char *str);
char						*get_value(char *str);
int							check_duplicates(char *str, t_env_list *env_list);
int							add_or_attach(char *str);
int							export_attach(char *str, t_env_list *env_list);

char						*convert_file_name(char *file_name, int i);
void						add_shell_level(t_env_list *env_list);

// Functions -- END

typedef enum FILE_CHECK // got the redirecting types for the files
{
	OUTFILE_NOT_USED,
	OUTFILE_USED,
	INFILE_NOT_USED,
	INFILE_USED
}							t_FILE_CHECK;

#endif
