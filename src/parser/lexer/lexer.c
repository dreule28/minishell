#include "minishell.h"

void	token_found(t_token_list *list, char *input, int *i, int start)
{
	char	*token_value;
	int		builtin_type;

	if (start != *i)
	{
		token_value = gc_substr(input, start, *i - start);
		builtin_type = get_bultin_type(token_value);
		add_token(list, token_value, builtin_type);
	}
}

void	handle_word_or_arg(t_token_list *list, char *input, int *i)
{
	int		start;
	int		in_word;
	char	quote;

	start = *i;
	in_word = 1;
	while (input[*i] && in_word)
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			quote = input[(*i)++];
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (!input[*i])
				error_message("Syntax error: Open quotes!\n");
			if (input[*i])
				(*i)++;
		}
		else if (input[*i] == ' ' || input[*i] == '\t'
		|| is_special_char(&input[*i]))
			in_word = 0;
		else
			(*i)++;
	}
	token_found(list, input, i, start);
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
		else
			handle_word_or_arg(list, input, &i);
	}
	return (list);
}
