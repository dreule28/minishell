#ifndef PARSER_H
# define PARSER_H

//Includes -- BEGIN
# include <stdio.h>
//Includes -- END

//Structs -- BEGIN
typedef enum type
{
	WORD,
	BUILTIN,
	S_QUOTES,
	D_QUOTES,
	INFILE,
	OUTFILE,
	HEREDOC,
	APPEND,
	PIPE
}			t_type;

typedef struct s_token
{
	t_type			token;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_token_list
{
	t_token	*head;
	t_token	*tail;
	int		size;
	char	*input;
}					t_token_list;
//Structs -- END

//Functions -- BEGIN
void	from_parser(void);
//Functions -- END

#endif
