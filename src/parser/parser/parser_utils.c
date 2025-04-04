#include "minishell.h"

bool	is_redirection(int token_type)
{
	return (token_type == TK_INFILE || token_type == TK_OUTFILE
		|| token_type == TK_HEREDOC || token_type == TK_APPEND);
}

bool	check_command(int token_type)
{
	return (token_type == TK_WORD || token_type == TK_BUILTIN
		|| token_type == S_QUOTES || token_type == D_QUOTES
		|| token_type == TK_AMPERSAND || token_type == TK_AND
		|| token_type == TK_OR);
}

void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	process_cmd_node(t_cmd_list *cmd_list, t_token *token, int cmd_type)
{
	char	**cmd;
	
	if (!cmd_list->tail)
	{
		DEBUG_INFO("No tail exists, creating new command node for: %s", token->value);
		cmd = ft_malloc(sizeof(char *) * 2, 1);
		if (!cmd)
		{
			DEBUG_ERROR("Memory allocation failed for command array");
			return;
		}
		cmd[0] = gc_strdup(token->value);
		if (!cmd[0])
		{
			DEBUG_ERROR("Failed to duplicate command value");
			free(cmd);
			return;
		}
		cmd[1] = NULL;
		if (!add_cmd_node(cmd_list, cmd, cmd_type))
		{
			DEBUG_ERROR("Failed to add command node");
			free(cmd[0]);
			free(cmd);
			return;
		}
		DEBUG_INFO("Added new command node with command: %s", cmd[0]);
	}
	else
	{
		DEBUG_INFO("Appending to existing command");
		append_arg(cmd_list, token);
	}
}
