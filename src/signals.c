#include "minishell.h"

volatile sig_atomic_t g_sigint_status = 0;

void	handle_sig_int(int signal_nb)
{
	(void)signal_nb;
	write(1, "\n", 1);
	if (g_sigint_status != 3)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		*exit_code() = 130;
	}
	g_sigint_status = 1;
	*exit_code() = 1;
}

void	remove_ctrlc_echo(void)
{
	struct termios	setting;

	tcgetattr(1, &setting);
	setting.c_lflag &= ~ ECHOCTL;
	tcsetattr(1, TCSAFLUSH, &setting);
}

void	handle_ctrlc_heredoc(int signal_nb)
{
	(void)signal_nb;
	g_sigint_status = 2;
	write(1, "\n", 1);
	rl_done = 1;
	ioctl(STDERR_FILENO, TIOCSTI, "\t");
}

void	start_heredoc_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;
	struct sigaction	sig_stop;

	sigemptyset(&sig_int.sa_mask);
	sigemptyset(&sig_quit.sa_mask);
	sigemptyset(&sig_stop.sa_mask);
	sig_int.sa_handler = handle_ctrlc_heredoc;
	sig_quit.sa_handler = SIG_IGN;
	sig_stop.sa_handler = SIG_IGN;
	sig_int.sa_flags = 0;
	sig_quit.sa_flags = 0;
	sig_stop.sa_flags = 0;
	if (sigaction(SIGINT, &sig_int, NULL) == -1
	|| sigaction(SIGQUIT, &sig_quit, NULL) == -1
	|| sigaction(SIGTSTP, &sig_stop, NULL) == -1)
	ft_putstr_fd("SIG_ERROR: Error while handling signals here\n", 2);
	remove_ctrlc_echo();
}

void	start_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;
	struct sigaction	sig_stop;

	sigemptyset(&sig_int.sa_mask);
	sigemptyset(&sig_quit.sa_mask);
	sigemptyset(&sig_stop.sa_mask);
	sig_int.sa_handler = handle_sig_int;
	sig_quit.sa_handler = SIG_IGN;
	sig_stop.sa_handler = SIG_IGN;
	sig_int.sa_flags = SA_RESTART;
	sig_quit.sa_flags = SA_RESTART;
	sig_stop.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sig_int, NULL) == -1||
		sigaction(SIGQUIT, &sig_quit, NULL) == -1
		|| sigaction(SIGTSTP, &sig_stop, NULL) == -1)
		ft_putstr_fd("SIG_ERROR: Error while handling signals\n", 2);
	remove_ctrlc_echo();
}
