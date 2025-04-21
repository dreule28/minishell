#include "minishell.h"

volatile sig_atomic_t g_exit_status = 0;

void	handle_sig_int(int signal_nb)
{
	(void)signal_nb;
	g_exit_status = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	start_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sigemptyset(&sig_int.sa_mask);
	sigemptyset(&sig_quit.sa_mask);
	sig_int.sa_handler = handle_sig_int;
	sig_quit.sa_handler = SIG_IGN;
	sig_int.sa_flags = SA_RESTART;
	sig_quit.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sig_int, NULL) == -1||
		sigaction(SIGQUIT, &sig_quit, NULL) == -1)
		ft_putstr_fd("SIG_ERROR: Error while handling signals\n", 2);
}
