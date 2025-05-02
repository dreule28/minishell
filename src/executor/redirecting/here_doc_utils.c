#include "minishell.h"

int	count_single_quotes(char *line)
{
	int	count;
	int	single_quote;

	count = 0;
	single_quote = 0;
	while (line[count] != '\0')
	{
		if (line[count] == 39)
			single_quote++;
		count++;
	}
	return (single_quote);
}

int	count_double_quotes(char *line)
{
	int	count;
	int	double_quote;

	count = 0;
	double_quote = 0;
	while (line[count] != '\0')
	{
		if (line[count] == 34)
			double_quote++;
		count++;
	}
	return (double_quote);
}

void	write_here_doc_file(char *line, int write_fd)
{
	ft_putstr_fd(line, write_fd);
	ft_putstr_fd("\n", write_fd);
}

void	write_here_doc(char *line, int write_fd, t_env_list *env_list,
		t_file_node *file_node)
{
	if (count_single_quotes(file_node->filename) == 2
		|| count_double_quotes(file_node->filename) == 2)
		write_here_doc_file(line, write_fd);
	else
		write_here_doc_file_expand(line, write_fd, env_list);
}
