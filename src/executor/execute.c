#include "minishell.h"

void execute(void)
{
    t_cmd_list *cmd_list;   // declare a pointer to a struct of type t_cmd_list
    init_structs(&cmd_list); // initialize the struct
    fill_structs(cmd_list);	// fill the struct with data
	print_structs(cmd_list); // print the struct
	access_infile(cmd_list); // check if the input file is accessible
	create_outfile(cmd_list); // create the output file
	append_file(cmd_list); // append the output file
}

