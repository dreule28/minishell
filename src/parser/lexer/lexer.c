#include "minishell.h"

void	handle_word_or_arg(t_token_list *list, char *input, int *i)
{
	int		start;
	char	*token_value;

	start = *i;
	while (input[*i] && !(input[*i] == ' ' || input[*i] == '\t')
		&& !is_special_char(&input[*i])
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	if (start != *i)
	{
		token_value = gc_substr(input, start, *i - start);
		add_token(list, token_value, TK_WORD);
	}
}

void	handle_builtins(t_token_list *list, char *input, int *i)
{
	int		start;
	char	*token_value;
	int		builtint_type;

	start = *i;
	while (input[*i] && !(input[*i] == ' ' || input[*i] == '\t')
		&& !is_special_char(&input[*i])
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	if (start != *i)
	{
		token_value = gc_substr(input, start, *i - start);
		builtint_type = get_bultin_type(token_value);
		add_token(list, token_value, builtint_type);
	}
}

t_token_list	*lexer(char *input)
{
	t_token_list	*list;
	int				i;

	i = 0;
	list = init_token_list(input);
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == '"' || input[i] == '\'')
			check_quotes(list, input, &i);
		else if (is_redir(&input[i]))
			check_redirs(list, input, &i);
		else if (input[i] == '&' || (input[i] == '|' && input[i + 1] == '|'))
			check_operators(list, input, &i);
		else if (input[i] == '|')
			handle_pipe(list, &i);
		else if (is_builtin(&input[i]))
			handle_builtins(list, input, &i);
		else
			handle_word_or_arg(list, input, &i);
	}
	return (list);
}
