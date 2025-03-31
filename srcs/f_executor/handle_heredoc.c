
#include "../../include/minishell.h"

bool	readline_till_EOF(t_minishell *shell, t_command *cmd, int *pipefd,
		size_t i)
{
	char	*line;
	char	*EOF_name;

	EOF_name = cmd->infiles[i].filename;
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (false);
		if (ft_strcmp(line, EOF_name) == 0)
			break ;
		if (cmd->infile_count == i + 1)
		{
			if (!printf_line_to_pipe(shell, pipefd[1], line))
				return (free(line), false);
		}
		free(line);
	}
	if (line)
		free(line);
	return (true);
}

bool	fprintf_to_tmpfile(t_minishell *shell, int fd, char *line)
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
		{
			shell->exit_status = EXIT_FAILURE;
			return (false);
		}
		free(line);
		line = tmp;
	}
	if (ft_fprintf(fd, "%s\n", line) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	return (true);
}

bool	process_heredoc_to_temp(t_minishell *shell, t_command *cmd, size_t i)
{
	char		tmpfile[] = "/tmp/minishell_heredoc_XXXXXX";
	int			fd;
	size_t		i;
	char		*line;
	const char	*delimiter;

	delimiter = cmd->infiles[i].filename;
	fd = mkstemp(tmpfile);
	if (fd == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (false);
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		if (!fprintf_to_tmpfile(shell, fd, line))
			return (free(line), close(fd), false);
	}
	close(fd);
	if (line)
		free(line);
	free(cmd->infiles[i].filename);
	cmd->infiles[i].filename = ft_strdup(tmpfile);
	if (!cmd->infiles[i].filename)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	return (true);
}

bool	close_and_dup2_pipe(t_minishell *shell, int *pipefd)
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		close(pipefd[0]);
		return (false);
	}
	close(pipefd[0]);
	return (true);
}

bool	handle_heredoc(t_minishell *shell)
{
	t_command	*current_cmd;
	size_t		i;

	current_cmd = shell->commands;
	setup_signals_heredoc();
	i = 0;
	while (current_cmd)
	{
		while (i < current_cmd->infile_count)
		{
			if (current_cmd->infiles[i].type == TOKEN_HEREDOC)
			{
				if (i + 1 == current_cmd->infile_count)
				{
					if (!process_heredocs_to_temp(shell, current_cmd, i))
						return (false);
					else
						heredoc_without_tmpfile();
				}
			}
			i++;
		}
		current_cmd = current_cmd->next;
	}
	if (!close_and_dup2_pipe(shell, pipefd))
		return (false);
	return (true);
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