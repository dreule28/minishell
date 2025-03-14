#include "minishell.h"

void	access_infile(t_cmd_list *cmd_list)
{
	t_file_node *current;

	if(cmd_list->head->files->head)
		current = cmd_list->head->files->head;
	else
		return;
	while(current != NULL)
	{
		current = current->next;
	}
}

void execute(void)
{
    t_cmd_list *cmd_list;
    init_structs(&cmd_list);
    fill_structs(cmd_list);
	print_structs(cmd_list);
	access_infile(cmd_list);

}

