

#include "../../include/minishell.h"

void	setup_signals_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("\0", 0);
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