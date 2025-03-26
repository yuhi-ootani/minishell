

#include "../../include/minishell.h"

static bool	is_last_heredoc(t_command *cmd, size_t i)
{
	return (cmd->is_heredoc && ft_strcmp(cmd->heredoc_files[i],
			cmd->input_file) == 0);
}

static void	readline_till_EOF(t_command *cmd, int *pipefd)
{
	char	*line;
	size_t	i;

	i = 0;
	while (i < cmd->heredoc_count)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, cmd->heredoc_files[i]) == 0)
		{
			free(line);
			i++;
			continue ;
		}
		if (is_last_heredoc(cmd, i))
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
		free(line);
	}
}

static void	handle_heredoc(t_command *cmd)
{
	int	pipefd[2];

	setup_signals_heredoc();
	if (pipe(pipefd) == -1)
	{
		perror("pipe in heredoc");
		exit(EXIT_FAILURE); // TODO
	}
	readline_till_EOF(cmd, pipefd);
	close(pipefd[1]);
	if (cmd->is_heredoc)
		dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

static void	input_redirection(t_command *cmd)
{
	int	input_fd;

	input_fd (cmd->input_file, O_RDONLY);
	if (input_fd < 0)
	{
		perror("input fd open");
		exit(EXIT_FAILURE);
	}
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
}

static void	output_redirection(t_command *cmd)
{
	int	flags;
	int	output_fd;

	flags = O_WRONLY | O_CREAT;
	if (cmd->is_append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	output_fd = open(cmd->out_file, flags, 0644);
	if (output_fd < 0)
	{
		perror("open output redirection file"); // modified
		exit(EXIT_FAILURE);                     // modified
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
}

void	handle_redirection(t_command *cmd)
{
	if (cmd->heredoc_count > 0)
		handle_heredoc(cmd);
	if (cmd->is_heredoc == false && cmd->input_file)
		input_redirection(cmd);
	if (cmd->out_file)
		output_redirection(cmd);
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