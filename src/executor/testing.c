#include "minishell.h"

void init_structs(t_cmd_list **cmd_list)
{
	(void)cmd_list;
}

void fill_structs(t_cmd_list *cmd_list)
{
    // ----- First Node -----
	// t_file_list *file_list;
	(void)cmd_list;
	// file_list = NULL;
	// // Allocate a new command node.


	// add_file_node(file_list ,"END12", HEREDOC);
	
	// add_file_node(file_list, "END12", HEREDOC);
	
	// add_file_node(file_list,"END12", HEREDOC);
	
	// add_file_node(file_list, "END12", HEREDOC);
    // // ----- Second Node -----
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


