#include "minishell.h"

void	check_quotes(t_token_list *list, char *input, int *i)
{
	int		start;
	char	quote;
	int		token_type;
	char	*token_value;

	quote = input[(*i)];
	start = *i;
	DEBUG_INFO("Processing quotes: %c at position %d", quote, start);
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("Syntax error: Open quotes!\n", 2);
		list->syntax_error = 1;
		return ;
	}
	(*i)++;
	if (quote == '"')
		token_type = D_QUOTES;
	else
		token_type = S_QUOTES;
	token_value = gc_substr(input, start, *i - start);
	add_token(list, token_value, token_type);
	DEBUG_INFO("Added %s token: %s", (token_type == D_QUOTES) ? "double quote" : "single quote", token_value); // ADD HERE
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
	add_token(list, gc_substr(input, start, *i - start), redir_typ);
}

int	check_redirs(t_token_list *list, char *input, int *i)
{
	int	redir_type;
	int	start;

	start = *i;
	redir_type = 0;
	DEBUG_INFO("Processing redirection at position %d", start); // ADD HERE
	check_and_assign(list, input, i, redir_type);
	DEBUG_INFO("Added redirection token type %d", list->tail->token); // ADD HERE after check_and_assign
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
			ft_putstr_fd("Syntax error: Missing filename!\n",2);
			list->syntax_error = 1;
			return (0);
		}
		add_token(list, gc_substr(input, start, *i - start), TK_WORD);
	}
	return (1);
}

void	handle_pipe(t_token_list *list, int *i)
{
	add_token(list, "|", TK_PIPE);
	(*i)++;
}
