#include "minishell.h"

void	check_quotes(t_token_list *list, char *input, int i)
{
	int		start;
	char	quote;

	quote = input[i++];
	start = i;
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
	{
		printf("Syntax error: Open quotes!\n");
		return ;
	}
	add_token(list, ft_substr(input, start, i - start), D_QUOTES);
	i++;
}

void	check_redirs(t_token_list *list, char *input, int i)
{
	int	redir_type;
	int	start;

	redir_type = get_redir_type(input, i);
	while (is_space(input[i]))
		i++;
	start = i;
	while (input[i] && !is_space(input[i]) && !is_special_char(input[i]))
		i++;
	if (start == i)
	{
		printf("Syntax error: Missing filename!");
		return ;
	}
	add_token(list, ft_substr(input, start, i - start), redir_type);
}

void	handle_word_or_arg(t_token_list *list, char *input, int i)
{
	int	start;

	start = i;
	while (input[i] && !is_space(input[i]) && !is_special_char(input[i]))
		i++;
	add_token(list, ft_substr(input, start, i - start), TK_WORD);
}

t_token_list	*lexer(char *input)
{
	t_token_list	*list;
	int				i;

	i = 0;
	list = init_token_list();
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break;
		if (input[i] == '"' || input[i] == '\'')
			check_quotes(list, input, &i);
		else if (is_redir(&input[i]))
			check_redirs(list, input, &i);
		else if (input[i] == '&' || (input[i] == '|' && input[i + 1] == '|'))
			check_operators(list, input, &i);
		else if (input[i] == '|')
		{
			add_token(list, "|", TK_PIPE);
			i++;
		}
		else
			handle_word_or_arg(list, input, &i);
	}
	return (list);
}
