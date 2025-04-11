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

int	handle_quotes(t_token_list *list, char *input, int *i)
{
	char	quote;

	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("Syntax error: Open quotes!\n",2 );
		list->syntax_error = 1;
		return (0);
	}
	(*i)++;
	return (1);
}

int	is_word_boundary(char *input, int *i)
{
	return (input[*i] == ' ' || input[*i] == '\t'
				|| is_special_char(&input[*i]));
}

void	handle_word_or_arg(t_token_list *list, char *input, int *i)
{
	int		start;
	int		in_word;

	start = *i;
	in_word = 1;
	while (input[*i] && in_word)
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			if (!handle_quotes(list, input, i))
				return ;
		}
		else if (is_word_boundary(input, i))
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
		else if (input[i] == '|')
			handle_pipe(list, &i);
		else
			handle_word_or_arg(list, input, &i);
		if (list->syntax_error)
		{
			DEBUG_ERROR("Syntax error detected, stopping lexer processing");
			break ;
		}
	}
	return (list);
}
