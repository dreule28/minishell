#include "minishell.h"

int file_redirecting_child(t_cmd_node *cmd_node, t_env_list *env_list)
{
    int file_check_outfile;
	int file_check_infile;

	file_check_infile = redir_loop_infiles(cmd_node, env_list);
	file_check_outfile = redir_loop_outfiles(cmd_node);
    if (file_check_outfile == -1 || file_check_infile == -1)
		return (-1);
	return (0);
}


int redir_loop_infiles(t_cmd_node *cmd_node, t_env_list *env_list)
{
	t_file_node *file_node;
	int infile_status;
	int return_value;

	return_value = 0;
	infile_status = INFILE_NOT_USED;
	if(cmd_node == NULL)
		return (INFILE_NOT_USED);
	file_node = cmd_node->files->head;
	while (file_node != NULL)
	{
		if (file_node->redir_type == TK_INFILE)
			return_value = redir_infile(file_node);
		else if (file_node->redir_type == TK_HEREDOC)
			return_value = redir_here_doc(file_node, env_list);
		if(return_value == -1)
			return(-1);
		if (return_value == INFILE_USED)
			infile_status = return_value;
		file_node = file_node->next;
	}
	return (infile_status);
}

int	redir_loop_outfiles(t_cmd_node *cmd_node)
{
	t_file_node *file_node;
	int outfile_status;
	int return_value;

	return_value = 0;
	outfile_status = OUTFILE_NOT_USED;
	if(cmd_node == NULL)
		return (OUTFILE_NOT_USED);
	file_node = cmd_node->files->head;
	while (file_node != NULL)
	{
		if (file_node->redir_type == TK_OUTFILE)
			return_value = redir_outfile(file_node);
		else if (file_node->redir_type == TK_APPEND)
			return_value = redir_append(file_node);
		if(return_value == -1)
			return(-1);
		if (return_value == OUTFILE_USED)
			outfile_status = return_value;
		file_node = file_node->next;
	}
	return (outfile_status);
}

void	delete_tmp_files(const char *folder_name)
{
	DIR *dir = opendir(folder_name);	
	char *file_name;
	char *tmp_str;
	struct dirent *file;

	while((file =readdir(dir)) != NULL)
	{
		if(file->d_name[0] == '.')
		{
			tmp_str = ft_strjoin(folder_name, "/");
			file_name = ft_strjoin(tmp_str, file->d_name);
			free(tmp_str);
			unlink(file_name);
			free(file_name);
		}
	}
	closedir(dir);
}

void	set_interaktive_line(void)
{
	int tty_fd;


    tty_fd = open("/dev/tty", O_RDWR);
    if (tty_fd == -1)
	{
		ft_putstr_fd("Could not open /dev/tty\n", 2);
		return ;
	}
    dup2(tty_fd, STDIN_FILENO);
    dup2(tty_fd, STDOUT_FILENO);
}
