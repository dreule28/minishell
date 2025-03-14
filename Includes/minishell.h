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


//Structs -- BEGIN
typedef struct s_file_node			// linked list for files that stores the filename and the redirect type (< , << , >> , >)
{
	char				*filename;	//	filename of the file that should be redirected	
	int					redir_type;	//	redirect type (HEREDOC, INFILE, OUTFILE and OUTFILE_APPEND)
	struct s_file_node	*next;		//	pointer to the next node
}	t_file_node;

typedef struct s_file_list			// list of filenames
{
	t_file_node	*head;				// head of filelist (pointing to a file)
	t_file_node	*tail;				// tail of filelist	(pointing to a file)
	ssize_t		size;				// size of filelist 
}	t_file_list;

typedef struct s_cmd_node			// struct that holds every information for the executing part
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


typedef enum CMD_TYPE				// got the cmd types (BULITIN  etc)
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
