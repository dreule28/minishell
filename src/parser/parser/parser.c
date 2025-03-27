#include "minishell.h"

void	handle_redirection(t_cmd_list *cmd_list, t_token *token)
{
	t_file_node	*file_node;

	if (!cmd_list || !cmd_list->tail || cmd_list->tail->files || !token)
		return ;
	file_node = create_file_node(token->value, token->token);
	if (!file_node)
		return ;
	add_file_node(cmd_list->tail->files, file_node);
}

void	append_arg(t_cmd_list *cmd_list, t_token *token)
{
	char	**new_cmd;
	int		i;

	i = 0;
	while (cmd_list->tail->cmd[i])
		i++;
	new_cmd = ft_malloc(sizeof(char *) * (i + 2), 1);
	if (!new_cmd)
		return ((void) NULL);
	i = 0;
	while (cmd_list->tail->cmd[i])
	{
		new_cmd[i] = cmd_list->tail->cmd[i];
		i++;
	}
	new_cmd[i] = ft_strdup(token->value);
	new_cmd[i + 1] = NULL;
	cmd_list->tail->cmd = new_cmd;
}

void	handle_command(t_cmd_list *cmd_list, t_token *token)
{
	int		cmd_type;
	char	**cmd;

	if (!cmd_list || !token)
		return ;
	cmd_type = CMD;
	if (token->token == TK_BUILTIN)
		cmd_type = TK_BUILTIN;
	if (!cmd_list->tail || !cmd_list->tail->cmd)
	{
		cmd = ft_malloc(sizeof(char *) * 2, 1);
		if (!cmd)
			return ((void) NULL);
		cmd[0] = ft_strdup(token->value);
		cmd[1] = NULL;
		add_cmd_node(cmd_list, cmd, cmd_type);
	}
	else
		append_arg(cmd_list, token);
}

t_cmd_list	*token_to_cmd(t_token_list *tk_list)
{
	t_cmd_list	*cmd_list;
	t_token		*curr;

	printf("\nStarting token to command conversion...\n");
	if (!tk_list || !tk_list->head)
	{
		printf("Error: Empty token list\n");
		return (NULL);
	}
	cmd_list = init_cmd_list();
	if (!cmd_list)
	{
		printf("Error: Failed to initialize command list\n");
		return (NULL);
	}
	curr = tk_list->head;
	while (curr)
	{
		printf("\nProcessing token: [%s] (type: %d)\n", curr->value, curr->token);
		if (curr->token == TK_PIPE)
		{
			printf("Adding pipe node\n");
			add_cmd_node(cmd_list, NULL, PIPE);
		}
		else if (is_redirection(curr->token))
		{
			printf("Handling redirection: %s\n", curr->value);
			handle_redirection(cmd_list, curr);
		}
		else
		{
			printf("Handling command/argument: %s\n", curr->value);
			handle_command(cmd_list, curr);
		}
		curr = curr->next;
	}
	printf("\nConversion complete. Printing final command list:\n");
	return (cmd_list);
}
