

#include "../../include/minishell.h"

bool	printf_line_to_pipe(t_minishell *shell, int fd, char *line)
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
		tmp = remove_quotes(shell, line);
		if (!tmp)
			return (false);
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

bool	init_pipe(t_minishell *shell, int *pipefd)
{
	if (pipe(pipefd) == -1)
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

t_token_type	last_infile_type(t_command *cmd)
{
	if (cmd->infile_count == 0)
		return (TOKEN_WORD);
	return (cmd->infiles[cmd->infile_count - 1].type);
}

bool	handle_heredoc(t_minishell *shell, t_command *cmd)
{
	int		pipefd[2];
	size_t	i;

	setup_signals_heredoc();
	if (!init_pipe(shell, pipefd))
		return (false);
	i = 0;
	while (i < cmd->infile_count)
	{
		if (cmd->infiles[i].type == TOKEN_HEREDOC)
		{
			if (!readline_till_EOF(shell, cmd, pipefd, i))
			{
				close(pipefd[0]);
				close(pipefd[1]);
				return (false);
			}
		}
		i++;
	}
	if (!close_and_dup2_pipe(shell, pipefd))
		return (false);
	return (true);
}

bool	input_redirection(t_minishell *shell, t_command *cmd)
{
	int		infile_fd;
	size_t	i;
	char	*filename;

	i = 0;
	while (i < cmd->infile_count)
	{
		if (cmd->infiles[i].type == TOKEN_REDIR_IN)
		{
			filename = cmd->infiles[i].filename;
			infile_fd = open(filename, O_RDONLY);
			if (infile_fd < 0)
			{
				fprintf(stderr, "MINISHELL: %s: %s\n", filename,
					strerror(errno));
				shell->exit_status = EXIT_FAILURE;
				return (false);
			}
			if (cmd->infile_count == i + 1)
				dup2(infile_fd, STDIN_FILENO);
			close(infile_fd);
		}
		i++;
	}
	return (true);
}

t_token_type	last_outfile_type(t_command *cmd)
{
	if (cmd->outfile_count == 0)
		return (TOKEN_WORD);
	return (cmd->outfiles[cmd->outfile_count - 1].type);
}

int	handle_open_flags(t_command *cmd, size_t i)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (i + 1 == cmd->outfile_count && last_outfile_type(cmd) == TOKEN_APPEND)
		flags |= O_APPEND;
	else if (i + 1 == cmd->outfile_count
		&& last_outfile_type(cmd) == TOKEN_REDIR_OUT)
		flags |= O_TRUNC;
	return (flags);
}

bool	output_redirection(t_minishell *shell, t_command *cmd)
{
	int		outfile_fd;
	size_t	i;
	char	*filename;
	int		flags;

	i = 0;
	while (i < cmd->outfile_count)
	{
		filename = cmd->outfiles[i].filename;
		flags = handle_open_flags(cmd, i);
		outfile_fd = open(filename, flags, 0644);
		if (outfile_fd < 0)
		{
			ft_fprintf(STDERR_FILENO, "MINISHELL: %s: %s\n", filename,
				strerror(errno));
			shell->exit_status = EXIT_FAILURE;
			return (false);
		}
		if (i + 1 == cmd->outfile_count)
			dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		i++;
	}
	return (true);
}

// fix to return bool later.
void	handle_redirection(t_minishell *shell, t_command *cmd)
{
	if (cmd->infile_count > 0)
	{
		if (!handle_heredoc(shell, cmd))
			return ;
		if (!input_redirection(shell, cmd))
			return ;
	}
	if (cmd->outfile_count > 0)
	{
		if (!output_redirection(shell, cmd))
			return ;
	}
}
// int	main(int argc, char **argv, char **envp)
// {
// 	const char *filename = "output.txt";
// 	const char *command_args[] = {"/bin/ls", NULL};

// 	int file;

// 	file = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
// 	dup2(file, STDOUT_FILENO);
// 	execve(command_args[0], command_args, envp);
// 	// write(file, text, ft_strlen(text));
// 	// write(file, "\n", 1);
// 	close(file);
// 	return (0);
// }