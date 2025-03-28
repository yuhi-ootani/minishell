

#include "../../include/minishell.h"

static bool	is_last_heredoc(t_command *cmd, size_t i)
{
	return (cmd->infile_count == i + 1);
}

static void	readline_till_EOF(t_command *cmd, int *pipefd, size_t i)
{
	char	*line;
	char	*EOF_name;

	EOF_name = cmd->infiles[i].filename;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, EOF_name) == 0)
		{
			free(line);
			break ;
		}
		if (is_last_heredoc(cmd, i))
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
		free(line);
	}
	// todo expand a heredoc input
}

t_token_type	last_infile_type(t_command *cmd)
{
	if (cmd->infile_count == 0)
		return (TOKEN_WORD);
	return (cmd->infiles[cmd->infile_count - 1].type);
}

static void	handle_heredoc(t_command *cmd)
{
	int		pipefd[2];
	size_t	i;
	bool	last_is_heredoc;

	last_is_heredoc = (TOKEN_HEREDOC == last_infile_type(cmd));
	setup_signals_heredoc();
	if (last_is_heredoc)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe in heredoc");
			exit(EXIT_FAILURE); // TODO
		}
	}
	i = 0;
	while (i < cmd->infile_count)
	{
		if (cmd->infiles[i].type == TOKEN_HEREDOC)
			readline_till_EOF(cmd, pipefd, i);
		i++;
	}
	if (last_is_heredoc)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
}

static void	input_redirection(t_command *cmd)
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
				exit(EXIT_FAILURE);
			}
			if (i + 1 == cmd->infile_count)
				dup2(infile_fd, STDIN_FILENO);
			close(infile_fd);
		}
		i++;
	}
}

void	output_redirection(t_command *cmd)
{
	int				outfile_fd;
	size_t			i;
	char			*filename;
	int				flags;
	t_token_type	type;

	i = 0;
	flags = O_WRONLY | O_CREAT;
	while (i < cmd->outfile_count)
	{
		filename = cmd->outfiles[i].filename;
		type = cmd->outfiles[i].type;
		if (i + 1 == cmd->outfile_count && type == TOKEN_APPEND)
			flags |= O_APPEND;
		else if (i + 1 == cmd->outfile_count && type == TOKEN_REDIR_OUT)
			flags |= O_TRUNC;
		outfile_fd = open(filename, flags, 0644);
		if (outfile_fd < 0)
		{
			perror("open output redirection file");
			exit(EXIT_FAILURE);
		}
		if (i + 1 == cmd->outfile_count)
			dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		i++;
	}
}

void	handle_redirection(t_command *cmd)
{
	if (cmd->infile_count > 0)
	{
		handle_heredoc(cmd);
		input_redirection(cmd);
	}
	if (cmd->outfile_count > 0)
	{
		output_redirection(cmd);
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