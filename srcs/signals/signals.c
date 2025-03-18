/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:41:58 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/18 19:23:17 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	handler(int status)
{
	if (status == SIGINT)
	{
		g_signal = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}


// volatile sig_atomic_t	g_signal = 0;

// void	disable_ctrlc_display(void)
// {
// 	struct termios	term;

// 	tcgetattr(STDIN_FILENO, &term);
// 	term.c_lflag &= ~ECHOCTL;
// 	tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }

// void	handle_sigint(int signum)
// {
// 	 disable_ctrlc_display();
// 	(void)signum;
// 	g_signal = 1;
// 	write(STDOUT_FILENO, "\n", 1);
// }

