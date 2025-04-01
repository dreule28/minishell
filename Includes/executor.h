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
void	execute(char **env);


// file_check.c 
int	redir_infile(t_cmd_list *cmd_list, int *pipe_fd);
int	redir_here_doc(t_cmd_list *cmd_list, int *pipe_fd);
int	redir_outfile(t_cmd_list *cmd_list, int *pipe_fd);
int	redir_append(t_cmd_list *cmd_list, int *pipe_fd);


// testing.c
void 	free_structs(t_cmd_list *cmd_list);
void	init_structs(t_cmd_list **cmd_list);
void	fill_structs(t_cmd_list *cmd_list);	
void	print_structs(t_cmd_list *cmd_list);


//Functions -- END


#endif
