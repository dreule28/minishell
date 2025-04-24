#ifndef MINISHELL_H
# define MINISHELL_H

//Includes -- BEGIN
# include <stdio.h>
# include <errno.h>
# include <dirent.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <termios.h>
# include "libft.h"
# include "parser.h"
# include "executor.h"
# include "debug.h"
# include "garbage_collector.h"
//Includes -- END
typedef struct s_file_node
{
	char				*filename;
	int					redir_type;
	struct s_file_node	*next;
}	t_file_node;

typedef struct s_file_list
{
	t_file_node	*head;
	t_file_node	*tail;
	ssize_t		size;
}	t_file_list;

typedef struct s_cmd_node
{
	struct s_cmd_node	*next;
	int					cmd_type;
	char				**cmd;
	t_file_list			*files;
}	t_cmd_node;

typedef struct s_cmd_list
{
	int			syntax_error;
	t_cmd_node	*head;
	t_cmd_node	*tail;
	ssize_t		size;
}	t_cmd_list;

typedef struct s_env_node
{
	char				*type;
	char				*value;
	struct s_env_node	*next;
}	t_env_node;

typedef struct s_env_list
{
	t_env_node	*head;
	t_env_node	*tail;
	ssize_t		size;
}	t_env_list;

//Structs -- BEGIN
typedef enum CMD_TYPE
{
	BUILTIN,
	CMD,
	PIPE
}	t_CMD_TYPE;

typedef enum REDIR_TYPE
{
	INFILE,
	OUTFILE,
	HEREDOC,
	OUTFILE_APPEND
}	t_REDIR_TYPE;
//Structs -- END

extern volatile sig_atomic_t	g_sigint_status;

//Functions -- BEGIN
int		*exit_code(void);
void	remove_ctrlc_echo(void);
void	handle_sig_int(int signal_nb);
void	start_heredoc_signals(void);
void	start_signals(void);
//Functions -- END

#endif
