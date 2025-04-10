

#include "../../include/minishell.h"

void	setup_signals_heredoc(void)
{
	struct sigaction sa;
    sa.sa_handler = sig_handler_heredoc;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		rl_done = 1;
	}
}

void	ignore_sigint(struct sigaction *original)
{
	struct sigaction	ignore;

	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	sigaction(SIGINT, &ignore, original);
}

void	restore_sigint(struct sigaction *original)
{
	sigaction(SIGINT, original, NULL);
}