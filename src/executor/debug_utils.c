#include "minishell.h"

int is_fd_open(int fd)
{
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}
void    check_fds(void)
{
    int check = 0;
    for(int i = 0; i < 21; i++)
    {
        check = is_fd_open(i);
        if (i < 3)
        {
            ft_putstr_fd("\033[33mfd ", 2);
            ft_putstr_fd(gc_itoa(i), 2);
            ft_putstr_fd(" is: \033[0m", 2);
        }
        else
        {
            ft_putstr_fd("fd ", 2);
            ft_putstr_fd(gc_itoa(i), 2);
            ft_putstr_fd(" is: ", 2);
        }
        if (check == 1)
            ft_putstr_fd("\033[32mopen\033[0m\n", 2);
        else
            ft_putstr_fd("\033[31mclosed\033[0m\n", 2);
    }
}