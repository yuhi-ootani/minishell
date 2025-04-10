

#include "../../include/minishell.h"

void	setup_signals_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
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
