#ifndef PARSER_H
# define PARSER_H

//Includes -- BEGIN
# include <stdio.h>
//Includes -- END

//Structs -- BEGIN
typedef struct s_file_node	t_file_node;
typedef struct s_file_list	t_file_list;
typedef struct s_cmd_node	t_cmd_node;
typedef struct s_cmd_list	t_cmd_list;
typedef struct s_env_node	t_env_node;
typedef struct s_env_list	t_env_list;

typedef enum type
{
	TK_WORD, //0
	TK_BUILTIN, //1
	S_QUOTES, //2 ''
	D_QUOTES, //3 ""
	TK_INFILE, //4 <
	TK_OUTFILE, //5 >
	TK_HEREDOC, //6 <<
	TK_APPEND, //7 >>
	TK_PIPE, //8 |
	TK_AMPERSAND, //9 &
	TK_AND, //10 &&
	TK_OR, //11 ||
}	t_type;

typedef enum seg_type
{
	SEG_NO_QUOTE,
	SEG_SINGLE,
	SEG_DOUBLE,
}	t_seg_type;

typedef struct s_segment
{
	char				*value;
	t_seg_type			type;
	struct s_segment	*next;
}	t_segment;

typedef struct s_segment_list
{
	t_segment	*head;
	t_segment	*tail;
	ssize_t		size;
}	t_segment_list;

typedef struct s_token
{
	t_type			token;
	char			*value;
	t_segment_list	*segment_list;
	struct s_token	*next;
}	t_token;

typedef struct s_token_list
{
	int		syntax_error;
	t_token	*head;
	t_token	*tail;
	ssize_t	size;
	char	*input;
}	t_token_list;
//Structs -- END

//Functions -- BEGIN
void			print_cmd_list(t_cmd_list *cmd_list);

//Lexer-FOLDER -- BEGIN
//check_actions.c -- BEGIN
void			check_quotes(t_token_list *list, char *input, int *i);
void			check_and_assign(t_token_list *list, char *input, int *i, int redir_typ);
int				check_redirs(t_token_list *list, char *input, int *i);
void			handle_pipe(t_token_list *list, int *i);
//check_actions.c -- END

//expansions.c -- BEGIN
void			handle_exit_code(t_segment *segment, char *beforer, int *i);
void			handle_rest(t_env_list *env_list, char *before, t_segment *segment, int *i);
void			expand_segment(t_segment *segment, t_env_list *env_list);
void			expand_token(t_token *token, t_env_list *env_list);
void			segment_tokens(t_token_list *tokens, t_env_list *env_list);
//expansions.c -- END

//expansion_utils.c -- BEGIN
void			handle_single_quotes(t_token *token, int *i, int *start);
void			handle_double_quotes(t_token *token, int *i, int *start);
char			*get_env_value(t_env_list *env_list, char *variable_name);
void			rebuild_token_value(t_token *token);
//expansion_utils.c -- END

//lexer_utils.c -- BEGIN
bool			is_redir(char *str);
int				get_redir_type(char *input, int i);
bool			is_special_char(const char *str);
int				get_bultin_type(const char *str);
//lexer_utils.c -- END

//lexer.c -- BEGIN
void			token_found(t_token_list *list, char *input, int *i, int start);
int				handle_quotes(t_token_list *list, char *input, int *i);
int				is_word_boundary(char *input, int *i);
void			handle_word_or_arg(t_token_list *list, char *input, int *i);
t_token_list	*lexer(char *input);
//lexer.c --END

//list_and_nodes.c -- BEGIN
t_token_list	*init_token_list(char *input);
void			add_token(t_token_list *list, char *token, int type);
t_segment		*create_segment(char *value, t_seg_type type);
t_segment_list	*init_segment_list(void);
//list_and_nodes.c -- END

//segments.c -- BEGIN
void			add_segment_to_list(t_segment_list *list, char *value, t_seg_type type);
void			add_segment_to_token(t_token *token, char *value, t_seg_type type);
void			split_token_into_segments(t_token *token);
void			split_token_into_heredocs(t_token *token);
t_token_list	*process_token_list(t_token_list *token_list);
//segments.c -- END

//Lexer-Folder -- END



//Parser-Folder -- BEGIN
//envs.c -- BEGIN
t_env_list		*init_env_list(void);
t_env_node		*add_env_node(char *type, char *value);
char			*find_equal_sign(char **env, int i, char **value);
t_env_list		*get_envs(char **env);
//envs.c -- END

//list_and_nodes_parser.c -- BEGIN
t_cmd_list		*init_cmd_list(void);
t_cmd_node		*add_cmd_node(t_cmd_list *cmd_list, char **cmd, int cmd_type);
t_file_node		*add_file_node(t_file_list *files, char *filename, int redir_type);
t_file_list		*init_file_list(void);
//list_and_nodes.c -- END

//token_to_cmd.c -- BEGIN
int				handle_redirection(t_cmd_list *cmd_list, t_token *token);
void			append_arg(t_cmd_list *cmd_list, t_token *token);
void			handle_command(t_cmd_list *cmd_list, t_token *token);
t_cmd_list		*token_to_cmd(t_token_list *tk_list);
//token_to_cmd.c -- END

//ttc_utils.c -- BEGIN
bool			is_redirection(int token_type);
bool			check_command(int token_type);
void			process_cmd_node(t_cmd_list *cmd_list, t_token *token, int cmd_type);
void			process_token(t_cmd_list *cmd_list, t_token **curr);
//ttc_utils.c -- END
//Parser-Folder -- END

//Functions -- END

#endif
