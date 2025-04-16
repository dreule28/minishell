#include "minishell.h"

void	transfer_cmd(char **new_cmd, char **old_cmd, int *i)
{
	while (old_cmd[(*i)])
	{
		new_cmd[(*i)] = gc_strdup(old_cmd[(*i)]);
		if (!new_cmd[(*i)])
			return ;
		(*i)++;
	}
}

int	handle_redirection(t_cmd_list *cmd_list, t_token *token)
{
	t_cmd_node	*cmd_node;
	t_file_node	*file_node;

	if (!cmd_list || !token || !token->next)
	{
		cmd_list->syntax_error = 1;
		return (0);
	}
	DEBUG_INFO("=== HANDLE REDIRECTION ===");
    DEBUG_INFO("Redirect operator: '%s' (type: %d)", token->value, token->token);
    DEBUG_INFO("Filename: '%s'", token->next->value);
    DEBUG_INFO("Current command count: %zu", cmd_list->size);
    DEBUG_INFO("Should we add a new node? NO - we should use existing node");
	if (cmd_list->size > 0 && cmd_list->tail)
	{
		cmd_node = cmd_list->tail;
		DEBUG_INFO("Using existing command node");
	}
	else
	{
		DEBUG_INFO("Creating new command node for redirection");
		cmd_node = add_cmd_node(cmd_list, NULL, CMD);
		if (!cmd_node)
			return (0);
	}
	file_node = add_file_node(cmd_node->files, token->next->value, token->token);
	if (!file_node)
		return (0);
	DEBUG_INFO("Added file '%s' to command node", token->next->value);
	return (1);
}

void	append_arg(t_cmd_list *cmd_list, t_token *token)
{
	char	**new_cmd;
	char	**old_cmd;
	int		i;

	if (!cmd_list || !cmd_list->tail || !cmd_list->tail->cmd || !token)
		return ;
	i = 0;
	while (cmd_list->tail->cmd[i])
		i++;
	new_cmd = ft_malloc(sizeof(char *), (i + 2));
	if (!new_cmd)
		return ;
	old_cmd = cmd_list->tail->cmd;
	i = 0;
	transfer_cmd(new_cmd, old_cmd, &i);
	new_cmd[i] = gc_strdup(token->value);
	if (!new_cmd[i])
	{
		cmd_list->tail->cmd = old_cmd;
		return;
	}
	new_cmd[i + 1] = NULL;
	cmd_list->tail->cmd = new_cmd;
}

void	handle_command(t_cmd_list *cmd_list, t_token *token)
{
	int		cmd_type;
	char	**cmd;

	if (!cmd_list || !token)
		return ;
	if (token->token == TK_BUILTIN)
		cmd_type = BUILTIN;
	else
		cmd_type = CMD;
	if (cmd_list->tail && !cmd_list->tail->cmd)
	{
		cmd = ft_malloc(sizeof(char *) * 2, 1);
		if (!cmd)
			return ;
		cmd[0] = gc_strdup(token->value);
		if (!cmd[0])
			return ;
		cmd[1] = NULL;
		cmd_list->tail->cmd = cmd;
	}
	else
		process_cmd_node(cmd_list, token, cmd_type);
}

t_cmd_list	*token_to_cmd(t_token_list *tk_list)
{
	t_cmd_list	*cmd_list;
	t_token		*curr;

	if (!tk_list || !tk_list->head)
		return (NULL);
	cmd_list = init_cmd_list();
	if (!cmd_list)
		return (NULL);
	curr = tk_list->head;
	while (curr)
	{
		process_token(cmd_list, &curr);
		if (cmd_list->syntax_error)
		{
			ft_putstr_fd("Syntax error: Learn syntax lol\n", 2);
			break ;
			// return (0);
		}
	}
	return (cmd_list);
}

// ls -l | grep .txt | echo "hello" | wc -l | < input