

#include "../../include/minishell.h"

static bool	is_last_heredoc(t_command *command, size_t i)
{
	return (command->is_heredoc && ft_strcmp(command->heredoc_files[i],
			command->input_file) == 0);
}

static void	readline_till_heredoc(t_command *command, int *pipefd)
{
	char	*line;
	size_t	i;

	i = 0;
	while (i < command->heredoc_count)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, command->heredoc_files[i]) == 0)
		{
			free(line);
			i++;
			continue ;
		}
		if (is_last_heredoc(command, i))
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
		free(line);
	}
}

static void	handle_heredoc(t_command *command)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe in heredoc");
		exit(EXIT_FAILURE); // TODO
	}
	readline_till_heredoc(command, pipefd);
	close(pipefd[1]);
	if (command->is_heredoc)
		dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

static void	input_redirection(t_command *command)
{
	int	input_fd;

	input_fd = open(command->input_file, O_RDONLY);
	if (input_fd < 0)
	{
		perror("input fd open");
		exit(EXIT_FAILURE);
	}
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
}

static void	output_redirection(t_command *command)
{
	int	flags;
	int	output_fd;

	flags = O_WRONLY | O_CREAT;
	if (command->is_append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	output_fd = open(command->out_file, flags, 0644);
	if (output_fd < 0)
	{
		perror("open output redirection file"); // modified
		exit(EXIT_FAILURE);                     // modified
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
}

void	handle_redirection(t_command *command)
{
	if (command->heredoc_count > 0)
		handle_heredoc(command);
	if (command->is_heredoc == false && command->input_file)
		input_redirection(command);
	if (command->out_file)
		output_redirection(command);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	const char *filename = "output.txt";
// 	const char *command_args[] = {"/bin/ls", NULL};

// 	int file;

// 	file = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
// 	dup2(file, STDOUT_FILENO);
// 	execve(command_args[0], command_args, envp);
// 	// write(file, text, strlen(text));
// 	// write(file, "\n", 1);
// 	close(file);
// 	return (0);
// }