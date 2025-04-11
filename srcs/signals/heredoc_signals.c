/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:53:54 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 14:15:55 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	setup_signals_heredoc(void)
// {
// 	// signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_IGN);
// }

void	sig_handler_heredoc(int sig)
{
	char	newline;

	if (sig == SIGINT)
	{
		newline = '\n';
		g_signal = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		// ioctl(STDIN_FILENO, TIOCSTI, &newline);
	}
}

void	setup_signals_heredoc(void)
{
	struct sigaction	sa;

	sa.sa_handler = sig_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
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
