#ifndef MINISHELL_H
# define MINISHELL_H

//Includes -- BEGIN
# include <stdio.h>
# include "parser.h"
# include "executor.h"
# include "garbage_collector.h"
//Includes -- END


//Structs -- BEGIN
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
	t_cmd_node	*head;
	t_cmd_node	*tail;
	ssize_t		size;
}	t_cmd_list;

typedef enum CMD_TYPE
{
	BUILTIN,
	CMD,
	PIPE
}	CMD_TYPE;

typedef enum REDIR_TYPE
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
