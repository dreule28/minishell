#ifndef EXECUTOR_H
# define EXECUTOR_H

//Includes -- BEGIN
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# include "minishell.h"
//Includes -- END

typedef struct s_cmd_list t_cmd_list;


//Functions -- BEGIN
void	execute(void);

void	access_infile(t_cmd_list *cmd_list);
void	create_outfile(t_cmd_list *cmd_list);
void	append_file(t_cmd_list *cmd_list);




//Functions -- END


#endif
