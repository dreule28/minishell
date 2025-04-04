#include "minishell.h"

void init_structs(t_cmd_list **cmd_list)
{
    *cmd_list = ft_malloc(1, sizeof(t_cmd_list));
    if (!*cmd_list)
        return;

    // Allocate the first node.
    (*cmd_list)->head = ft_malloc(1, sizeof(t_cmd_node));
    if (!(*cmd_list)->head)
        return;
    
    // Initialize the file list for the first node
    (*cmd_list)->head->files = ft_malloc(1, sizeof(t_file_list));
    if (!(*cmd_list)->head->files)
        return;
    (*cmd_list)->head->files->head = NULL;
    (*cmd_list)->head->files->tail = NULL;
    (*cmd_list)->head->files->size = 0;
    
    // Initialize the first command node fields
    (*cmd_list)->head->cmd = NULL;
    (*cmd_list)->head->cmd_type = 0;
    (*cmd_list)->head->next = NULL;  // assuming your t_cmd_node has a next pointer

    // Initialize the t_cmd_list structure.
    (*cmd_list)->tail = (*cmd_list)->head;
    (*cmd_list)->size = 1;
}

void fill_structs(t_cmd_list *cmd_list)
{
    // ----- First Node -----
	t_file_node *first_file_node;
	t_file_node *second_file_node;
	t_file_node *third_file_node;
	t_file_node *fourth_file_node;
	// Allocate a new command node.
	add_cmd_node(cmd_list, NULL, CMD);

	first_file_node = create_file_node("END12", HEREDOC);
	add_file_node(cmd_list->head->files, first_file_node);

	second_file_node = create_file_node("END2", HEREDOC);
	add_file_node(cmd_list->head->files, second_file_node);
	
	third_file_node = create_file_node("END3", HEREDOC);
	add_file_node(cmd_list->head->files, third_file_node);
	
	fourth_file_node = create_file_node("END4", HEREDOC);
	add_file_node(cmd_list->head->files, fourth_file_node);
    // ----- Second Node -----
	// t_file_node *second_file_node;
    // Allocate a new command node.

	// add_cmd_node(cmd_list, NULL, CMD);
	// second_file_node =  create_file_node("new_outfile2.txt", OUTFILE);
	// add_file_node(cmd_list->tail->files, second_file_node);
	// ----- Third Node -----
	// Allocate a new command node.

}


void print_structs(t_cmd_list *cmd_list)
{
    t_cmd_node *node = cmd_list->head;
    while (node)
    {
        printf("Command: ");
        for (int i = 0; node->cmd && node->cmd[i]; i++)
            printf("%s ", node->cmd[i]);
        printf("\n");

        // Print file redirection info if available.
        if (node->files && node->files->head)
        {
            printf("Filename: %s\n", node->files->head->filename);
            printf("Redir type: %d\n", node->files->head->redir_type);
        }
        printf("\n");
        node = node->next;
    }
}

// void setup_env_list()
// {

// }

