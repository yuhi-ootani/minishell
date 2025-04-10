/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:28:50 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/10 15:09:15 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*create_tmpfile_path(t_minishell *shell)
{
	char		*tmpfile_path;
	static int	i = 0;
	char		*tmpfile_index;
	char		*result;

	tmpfile_path = "/tmp/minishell_heredoc_";
	tmpfile_index = ft_itoa(i);
	if (i == INT_MAX)
		i = 0;
	else
		i++;
	if (!tmpfile_index)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	result = ft_strjoin(tmpfile_path, tmpfile_index);
	free(tmpfile_index);
	if (!result)
		shell->exit_status = EXIT_FAILURE;
	return (result);
}

static bool	handle_heredoc_status(t_minishell *shell, char **filename)
{
	int	status;

	status = shell->exit_status;
	(void)filename; // clean
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		// unlink(*filename);
		// free(*filename);
		// *filename = NULL;
		shell->exit_status = 130;
		return (false);
	}
	else if (WEXITSTATUS(status) != 0)
	{
		// unlink(*filename);
		// free(*filename);
		// *filename = NULL;
		return (false);
	}
	return (true);
}

bool	start_heredoc_process(t_minishell *shell, t_command *cmd, size_t i)
{
	pid_t				pid;
	struct sigaction	sa_original;
	char				*eof_name;

	eof_name = cmd->infiles[i].filename;
	cmd->infiles[i].filename = create_tmpfile_path(shell);
	if (!cmd->infiles[i].filename)
		return (free(eof_name), false);
	ignore_sigint(&sa_original);
	pid = fork();
	if (pid == -1)
		return (restore_sigint(&sa_original), free(eof_name),
			set_exit_failure(shell), false);
	else if (pid == 0)
		child_heredoc(shell, cmd->infiles[i].filename, eof_name);
	waitpid(pid, &shell->exit_status, 0);
	restore_sigint(&sa_original);
	free(eof_name);
	return (handle_heredoc_status(shell, &cmd->infiles[i].filename));
}

bool	handle_heredoc(t_minishell *shell)
{
	t_command	*current_cmd;
	size_t		i;

	current_cmd = shell->commands;
	while (current_cmd)
	{
		i = 0;
		while (i < current_cmd->infile_count)
		{
			if (current_cmd->infiles[i].type == TOKEN_HEREDOC)
			{
				if (!start_heredoc_process(shell, current_cmd, i))
					return (false);
			}
			i++;
		}
		current_cmd = current_cmd->next;
	}
	return (true);
}

void	clean_heredoc_tmpfile(t_minishell *shell)
{
	size_t		i;
	t_command	*cmd;

	i = 0;
	cmd = shell->commands;
	while (cmd)
	{
		while (i < cmd->infile_count)
		{
			if (cmd->infiles[i].type == TOKEN_HEREDOC)
			{
				unlink(cmd->infiles[i].filename);
			}
			i++;
		}
		cmd = cmd->next;
	}
}

// ▗▄▄▖▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖    ▗▖  ▗▖▗▄▄▄▖▗▄▄▖  ▗▄▄▖▗▄▄▄▖ ▗▄▖ ▗▖  ▗▖
// ▐▌ ▐▌ █  ▐▌ ▐▌▐▌       ▐▌  ▐▌▐▌   ▐▌ ▐▌▐▌     █  ▐▌ ▐▌▐▛▚▖▐▌
// ▐▛▀▘  █  ▐▛▀▘ ▐▛▀▀▘    ▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖ ▝▀▚▖  █  ▐▌ ▐▌▐▌ ▝▜▌
// ▐▌  ▗▄█▄▖▐▌   ▐▙▄▄▖     ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌▗▄▄▞▘▗▄█▄▖▝▚▄▞▘▐▌  ▐▌

// bool	printf_line_to_pipe(t_minishell *shell, int fd, char *line)
// {
// 	char	*tmp;

// 	tmp = NULL;
// 	if (ft_strchr(line, '$'))
// 	{
// 		tmp = get_expanded_str(shell, line);
// 		if (!tmp)
// 			return (false);
// 		free(line);
// 		line = tmp;
// 		tmp = strdup_except_quotes_util(line);
// 		if (!tmp)
// 		{
// 			shell->exit_status = EXIT_FAILURE;
// 			return (false);
// 		}
// 		free(line);
// 		line = tmp;
// 	}
// 	if (ft_fprintf(fd, "%s\n", line) == -1)
// 	{
// 		shell->exit_status = EXIT_FAILURE;
// 		return (false);
// 	}
// 	return (true);
// }

// bool	readline_till_EOF(t_minishell *shell, t_command *cmd, int *pipefd,
// 		size_t i)
// {
// 	char	*line;
// 	char	*EOF_name;

// 	EOF_name = cmd->infiles[i].filename;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			return (false);
// 		if (ft_strcmp(line, EOF_name) == 0)
// 			break ;
// 		if (cmd->infile_count == i + 1)
// 		{
// 			if (!printf_line_to_pipe(shell, pipefd[1], line))
// 				return (free(line), false);
// 		}
// 		free(line);
// 	}
// 	if (line)
// 		free(line);
// 	return (true);
// }

// bool	close_and_dup2_pipe(t_minishell *shell, int *pipefd)
// {
// 	close(pipefd[1]);
// 	if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 	{
// 		shell->exit_status = EXIT_FAILURE;
// 		close(pipefd[0]);
// 		return (false);
// 	}
// 	close(pipefd[0]);
// 	return (true);
// }

// bool	init_pipe(t_minishell *shell, int *pipefd)
// {
// 	if (pipe(pipefd) == -1)
// 	{
// 		shell->exit_status = EXIT_FAILURE;
// 		return (false);
// 	}
// 	return (true);
// }

// bool	handle_heredoc(t_minishell *shell, t_command *cmd)
// {
// 	int		pipefd[2];
// 	size_t	i;

// 	setup_signals_heredoc();
// 	if (!init_pipe(shell, pipefd))
// 		return (false);
// 	i = 0;
// 	while (i < cmd->infile_count)
// 	{
// 		if (cmd->infiles[i].type == TOKEN_HEREDOC)
// 		{
// 			if (!readline_till_EOF(shell, cmd, pipefd, i))
// 			{
// 				close(pipefd[0]);
// 				close(pipefd[1]);
// 				return (false);
// 			}
// 		}
// 		i++;
// 	}
// 	if (!close_and_dup2_pipe(shell, pipefd))
// 		return (false);
// 	return (true);
// }
