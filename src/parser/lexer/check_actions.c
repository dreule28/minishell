#include "minishell.h"

void	check_quotes(t_token_list *list, char *input, int *i)
{
	int		start;
	char	quote;
	int		token_type;
	char	*token_value;

	quote = input[(*i)];
	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		printf("Syntax error: Open quotes!\n");
		return ;
	}
	(*i)++;
	if (quote == '"')
		token_type = D_QUOTES;
	else
		token_type = S_QUOTES;
	token_value = ft_substr(input, start, *i - start);
	add_token(list, token_value, token_type);
}

void	check_operators(t_token_list *list, char *input, int *i)
{
	int		start;
	int		token_type;

	start = *i;
	token_type = 0;
	if (input[*i] == '&' && input[*i + 1] == '&')
	{
		token_type = TK_AND;
		(*i) += 2;
	}
	else if (input[*i] == '|' && input[*i + 1] == '|')
	{
		token_type = TK_OR;
		(*i) += 2;
	}
	else if (input[*i] == '&')
	{
		token_type = TK_AMPERSAND;
		(*i)++;
	}
	if (token_type != 0)
		add_token(list, ft_substr(input, start, *i - start), token_type);
}

void	check_and_assign(t_token_list *list, char *input, int *i, int redir_typ)
{
	int	start;

	start = *i;
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		redir_typ = TK_HEREDOC;
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		redir_typ = TK_APPEND;
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		redir_typ = TK_INFILE;
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		redir_typ = TK_OUTFILE;
		(*i)++;
	}
	add_token(list, ft_substr(input, start, *i - start), redir_typ);
}

void	check_redirs(t_token_list *list, char *input, int *i)
{
	int	redir_type;
	int	start;

	start = *i;
	redir_type = 0;
	check_and_assign(list, input, i, redir_type);
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	start = *i;
	if (input[*i] == '"' || input[*i] == '\'')
		check_quotes(list, input, i);
	else
	{
		while (input[*i] && !(input[*i] == ' ' || input[*i] == '\t')
			&& !is_special_char(&input[*i]))
			(*i)++;
		if (start == *i)
		{
			printf("Syntax error: Missing filename!\n");
			return ;
		}
		add_token(list, ft_substr(input, start, *i - start), TK_WORD);
	}
}
