#ifndef PARSER_H
# define PARSER_H

//Includes -- BEGIN
# include <stdio.h>
//Includes -- END

//Structs -- BEGIN
typedef enum type
{
	TK_WORD,
	TK_BUILTIN,
	S_QUOTES,
	D_QUOTES,
	TK_INFILE,
	TK_OUTFILE,
	TK_HEREDOC,
	TK_APPEND,
	TK_PIPE
}			t_type;

typedef struct s_token
{
	// t_type			token;
	struct s_token	*next;
	char			*value;
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
t_token_list	*lexer(char *input);
t_token_list	*init_token_list();
void			add_token(t_token_list *list, char *token, int type);
bool			is_redir(char *str);
int				get_redir_type(char *input, int i);
bool			is_special_char(char c);
void			check_quotes(t_token_list *list, char *input, int i);
void			check_redirs(t_token_list *list, char *input, int i);
void			handle_word_or_arg(t_token_list *list, char *input, int i);
//Functions -- END

#endif
