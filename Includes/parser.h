#ifndef PARSER_H
# define PARSER_H

//Includes -- BEGIN
# include <stdio.h>
//Includes -- END

//Structs -- BEGIN
typedef struct s_file_node t_file_node;
typedef struct s_file_list t_file_list;
typedef struct s_cmd_node t_cmmd_node;
typedef struct s_cmd_list t_cmd_list;
typedef struct s_env_node t_env_node;
typedef struct s_env_list t_env_list;
typedef enum type
{
	TK_WORD, //0
	TK_BUILTIN, //1
	S_QUOTES, //2
	D_QUOTES, //3
	TK_INFILE, //4
	TK_OUTFILE, //5
	TK_HEREDOC, //6
	TK_APPEND, //7
	TK_PIPE, //8
	TK_AMPERSAND, //9
	TK_AND, //10
	TK_OR, //11
}			t_type;

typedef struct s_token
{
	t_type			token;
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
void			 print_cmd_list(t_cmd_list *cmd_list);
//Lexer-FOLDER -- BEGIN
//check_actions.c -- BEGIN
void			check_quotes(t_token_list *list, char *input, int *i);
void			check_operators(t_token_list *list, char *input, int *i);
void			check_and_assign(t_token_list *list, char *input, int *i,
					int redir_typ);
void			check_redirs(t_token_list *list, char *input, int *i);
void			handle_pipe(t_token_list *list, int *i);
//check_actions.c -- END

//lexer_utils.c -- BEGIN
bool			is_redir(char *str);
int				get_redir_type(char *input, int i);
bool			is_special_char(const char *str);
bool			is_builtin(const char *str);
int				get_bultin_type(const char *str);
//lexer_utils.c -- END

//lexer.c -- BEGIN
void			handle_word_or_arg(t_token_list *list, char *input, int *i);
t_token_list	*lexer(char *input);
void			handle_builtins(t_token_list *list, char *input, int *i);
//lexer.c --END

//list_and_nodes.c -- BEGIN
t_token_list	*init_token_list(char *input);
void			add_token(t_token_list *list, char *token, int type);
//list_and_nodes.c -- END
//Lexer-Folder -- END



//Parser-Folder -- BEGIN
//envs.c -- BEGIN
t_env_list		*init_env_list(void);
t_env_node		*add_env_node(char *type, char *value);
char			*find_equal_sign(char **env, int i, char **value);
void			get_envs(char **env);
//envs.c -- END

//list_and_nodes.c -- BEGIN
t_cmd_list		*init_cmd_list(void);
void			add_cmd_node(t_cmd_list *cmd_list, char **cmd, int cmd_type);
t_file_node		*create_file_node(char *filename, int redir_type);
void			add_file_node(t_file_list *files, t_file_node *node);
//list_and_nodes.c -- END

//parser_utils.c -- BEGIN
bool			is_redirection(int token_type);
bool			check_command(int token_type);
void			free_str_array(char **array);
//parser_utils.c -- END

//parser.c -- BEGIN
void			handle_redirection(t_cmd_list *cmd_list, t_token *token);
void			append_arg(t_cmd_list *cmd_list, t_token *token);
void			handle_command(t_cmd_list *cmd_list, t_token *token);
t_cmd_list		*token_to_cmd(t_token_list *tk_list);
//parser.c -- END
//Parser-Folder -- END

//Functions -- END

#endif
