/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:41:58 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/24 11:51:16 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

// void	handler(int status)
// {
// 	if (status == SIGINT)
// 	{
// 		g_signal = 1;
// 		write(STDOUT_FILENO, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }


// void	setup_signals_parent(void)
// {
// 	struct sigaction	sa;

// 	sa.sa_handler = handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// }

// void	setup_signals_child(void)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// }
// void	setup_signals(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_handler = handler;
// 	sa.sa_flags = SA_RESTART;
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// }


void	sig_handler_parent(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_signals_parent(void)
{
	struct sigaction	sa;

	sa.sa_handler = sig_handler_parent;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}
