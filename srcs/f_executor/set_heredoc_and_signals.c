/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_heredoc_and_signals.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:31:11 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 13:16:26 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ignore_sigint(struct sigaction *original)
{
	struct sigaction	ignore;

	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	sigaction(SIGINT, &ignore, original);
}

static void	restore_sigint(struct sigaction *original)
{
	sigaction(SIGINT, original, NULL);
}

static void	child_heredoc(t_minishell *shell, char *filename, char *eof_name)
{
	int	fd;

	sigaction(SIGINT, NULL, NULL);
	setup_signals_heredoc();
	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		exit(EXIT_FAILURE);
	if (!readline_till_eof(shell, eof_name, fd))
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}

static bool	handle_heredoc_status(t_minishell *shell, int status,
		char **filename)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		unlink(*filename);
		free(*filename);
		*filename = NULL;
		shell->exit_status = 130;
		return (false);
	}
	else if (WEXITSTATUS(status) != 0)
	{
		unlink(*filename);
		free(*filename);
		*filename = NULL;
		return (false);
	}
	return (true);
}

bool	start_heredoc_process(t_minishell *shell, t_command *cmd, size_t i)
{
	pid_t				pid;
	int					status;
	struct sigaction	sa_original;
	char				*eof_name;

	eof_name = cmd->infiles[i].filename;
	cmd->infiles[i].filename = create_tmpfile_path(shell);
	if (!cmd->infiles[i].filename)
		return (set_exit_failure(shell), false);
	ignore_sigint(&sa_original);
	pid = fork();
	if (pid == -1)
		return (restore_sigint(&sa_original), perror("fork"), false);
	else if (pid == 0)
		child_heredoc(shell, cmd->infiles[i].filename, eof_name);
	waitpid(pid, &status, 0);
	restore_sigint(&sa_original);
	free(eof_name);
	return (handle_heredoc_status(shell, status, &cmd->infiles[i].filename));
}
