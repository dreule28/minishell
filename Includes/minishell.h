#ifndef MINISHELL_H
# define MINISHELL_H

//Includes -- BEGIN
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "parser.h"
# include "executor.h"
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
	struct s_cmd_node	*next;		// pointer to the next node
	int					cmd_type;	// Token (BUILDIN, CMD, PIPE,  etc)
	char				**cmd;		// array of the cmd (ls, -al) or (wc, -l)
	t_file_list			*files;		// linked list of files ??
}	t_cmd_node;

typedef struct s_cmd_list
{
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
typedef enum CMD_TYPE				// got the cmd types (BULITIN  etc)1
{
	BUILTIN,
	CMD,
	PIPE
}	CMD_TYPE;

typedef enum REDIR_TYPE				// got the redirecting types for the files
{
	INFILE,
	OUTFILE,
	HEREDOC,
	OUTFILE_APPEND
}	REDIR_TYPE;
//Structs -- END

//Functions -- BEGIN
//Functions -- END

#endif
