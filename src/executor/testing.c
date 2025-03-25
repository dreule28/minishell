#include "minishell.h"

void init_structs(t_cmd_list **cmd_list)
{
    *cmd_list = ft_malloc(1, sizeof(t_cmd_list));
    if (!*cmd_list)
        return;

    (*cmd_list)->head = ft_malloc(1, sizeof(t_cmd_node));
    if (!(*cmd_list)->head)
        return;

    (*cmd_list)->head->files = ft_malloc(1, sizeof(t_file_list));
    if (!(*cmd_list)->head->files)
        return;

    // Initialize the t_file_list structure
    (*cmd_list)->head->files->head = NULL;
    (*cmd_list)->head->files->tail = NULL;
    (*cmd_list)->head->files->size = 0;

    // Initialize the t_cmd_node structure
    (*cmd_list)->head->cmd = NULL;
    (*cmd_list)->head->cmd_type = 0;

    // Initialize the t_cmd_list
    (*cmd_list)->tail = (*cmd_list)->head;
    (*cmd_list)->size = 1;
}

void fill_structs(t_cmd_list *cmd_list)
{

	// ls -al >outfile
	// TOKEN (cmd_type)
	
	cmd_list->head->cmd_type = CMD;
	
	// CMD
	char **cmd_arr;
	
	char *input = "ls -l";
	
	cmd_arr = ft_split(input, ' ');
	
	cmd_list->head->cmd = cmd_arr; 
	
	// FILENAME

	cmd_list->head->files->head = ft_malloc(1, sizeof(t_file_node));
	if (!cmd_list->head->files->head)
    	return;
	cmd_list->head->files->head->filename = ft_strdup("END");
	gc_add(cmd_list->head->files->head->filename);

	// FILE REDIR

	cmd_list->head->files->head->redir_type = HEREDOC;
}

void	print_structs(t_cmd_list *cmd_list)
{
	printf("\n");
	printf("prompt :			 'ls -al <<END \n");
	printf("int	cmd_type:		 %d\n", cmd_list->head->cmd_type);
	printf("char **cmd :			 {%s, %s, NULL}\n", cmd_list->head->cmd[0], cmd_list->head->cmd[1]);
	printf("char *filename:			 %s\n", cmd_list->head->files->head->filename);
	printf("int	redir_type:		 %d\n", cmd_list->head->files->head->redir_type);
	printf("\n");
}

