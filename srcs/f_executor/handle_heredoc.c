/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:28:50 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 20:53:58 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	fprintf_to_tmpfile(t_minishell *shell, char *line, int fd)
{
	char	*tmp;

	tmp = NULL;
	if (ft_strchr(line, '$'))
	{
		tmp = get_expanded_str(shell, line);
		if (!tmp)
			return (false);
		free(line);
		line = tmp;
		tmp = strdup_except_quotes_util(line);
		if (!tmp)
			return (set_exit_failure(shell), false);
		free(line);
		line = tmp;
	}
	if (ft_fprintf(fd, "%s\n", line) == -1)
		return (set_exit_failure(shell), false);
	return (true);
}

// bool	readline_till_eof(t_minishell *shell, const char *eof_name, int fd)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			return (false);
// 		if (ft_strcmp(line, eof_name) == 0)
// 			break ;
// 		if (!fprintf_to_tmpfile(shell, line, fd))
// 			return (free(line), false);
// 		free(line);
// 	}
// 	free(line);
// 	return (true);
// }

bool	readline_till_eof(t_minishell *shell, const char *eof_name, int fd)
{
	char				*line;
	struct sigaction	sa;
	struct sigaction	old;

	g_signal = 0;
	sa.sa_handler = sig_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, &old);
	while (1)
	{
		line = readline("> ");
		if (!line || g_signal)
		{
			sigaction(SIGINT, &old, NULL);
			shell->exit_status = 1;
			free(line);
			return (false);
		}
		if (!line || ft_strcmp(line, eof_name) == 0)
			break ;
		if (!fprintf_to_tmpfile(shell, line, fd))
		{
			free(line);
			sigaction(SIGINT, &old, NULL);
			return (false);
		}
		free(line);
	}
	sigaction(SIGINT, &old, NULL);
	free(line);
	return (true);
}

char	*create_tmpfile_path(t_minishell *shell)
{
	char		tmpfile_path[] = "/tmp/minishell_heredoc_";
	static int	i = 0;
	char		*tmpfile_index;
	char		*result;

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

bool	start_heredoc_process(t_minishell *shell, t_command *cmd, size_t i)
{
	int		fd;
	char	*eof_name;

	eof_name = cmd->infiles[i].filename;
	cmd->infiles[i].filename = create_tmpfile_path(shell);
	if (!cmd->infiles[i].filename)
		return (set_exit_failure(shell), false);
	fd = open(cmd->infiles[i].filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (set_exit_failure(shell), false);
	if (!readline_till_eof(shell, eof_name, fd))
		return (close(fd), free(eof_name), false);
	close(fd);
	free(eof_name);
	return (true);
}

bool	handle_heredoc(t_minishell *shell)
{
	t_command	*current_cmd;
	size_t		i;

	current_cmd = shell->commands;
	setup_signals_heredoc();
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