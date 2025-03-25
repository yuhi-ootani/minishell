/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:16:42 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/24 18:28:04 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// // int	main(void)
// // {
// // 	pid_t	pid;
// // 	int		status;

// // 	for (int i = 0; i < 3; i++)
// // 	{
// // 		pid = fork();
// // 		if (pid == 0)
// // 		{
// // 			printf("Child %d (PID: %d) started\n", i, getpid());
// // 			sleep(20 - i); // Different exit times
// // 			printf("Child %d (PID: %d) exiting\n", i, getpid());
// // 			exit(0);
// // 		}
// // 	}
// // 	// Parent waits for all children
// // 	while (wait(&status) > 0)
// // 		printf("A child has exited\n");
// // 	printf("All children have finished.\n");
// // 	return (0);
// // }

// #include "../../include/minishell.h"
// #include "../../libft/libft.h"
// #include <readline/history.h>
// #include <readline/readline.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/wait.h>
// #include <unistd.h>

// /*
//  * heredoc:
//  * Reads lines from the user until the delimiter is entered.
//  * Writes each line to the write end of a pipe.
//  * Returns the read end of the pipe containing the heredoc content.
//  */
// int	heredoc(char *delimiter)
// {
// 	int		pipefd[2];
// 	pid_t	pid;
// 	char	*line;
// 	int		status;

// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		while (1)
// 		{
// 			line = readline("> ");
// 			if (!line)
// 				break ; // End-of-file (Ctrl-D) or error.
// 			if (strcmp(line, delimiter) == 0)
// 			{
// 				free(line);
// 				break ; // Delimiter reached.
// 			}
// 			write(pipefd[1], line, ft_strlen(line));
// 			write(pipefd[1], "\n", 1);
// 			free(line);
// 		}
// 		close(pipefd[1]); // Close write end in the child.
// 		exit(EXIT_SUCCESS);
// 	}
// 	else
// 	{                             // Parent process.
// 		close(pipefd[1]);         // Parent only needs the read end.
// 		return (pipefd[0]);       // Return the read end of the pipe.
// 	}
// }

// /*
//  * print_heredoc_contents:
//  * Reads the entire content from the given file descriptor (fd)
//  * and prints it to standard output.
//  *
//  * Warning: This will consume the contents from fd.
//  */
// void	print_heredoc_contents(int fd)
// {
// 	char	buffer[1024];
// 	ssize_t	bytes_read;

// 	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
// 	{
// 		write(STDOUT_FILENO, buffer, bytes_read);
// 	}
// 	if (bytes_read < 0)
// 	{
// 		perror("read");
// 	}
// }

// int	main(void)
// {
// 	char	*delimiter;
// 	int		heredoc_fd;

// 	delimiter = "EOF";
// 	heredoc_fd = heredoc(delimiter);
// 	// Print the contents of the heredoc.
// 	printf("Heredoc contents:\n");
// 	print_heredoc_contents(heredoc_fd);
// 	// If you intend to use the heredoc as input for a command (via dup2),
// 	// you would need to capture and store the data before printing it,
// 	// because reading from the fd here consumes the input.
// 	close(heredoc_fd);
// 	return (0);
// }

/*
 * heredoc:
 * Reads lines from the user until the delimiter is entered.
 * Writes each line to the write end of a pipe.
 * Returns the read end of the pipe containing the heredoc content.
 */
int	heredoc(char *delimiter)
{
	int		pipefd[2];
	pid_t	pid;
	char	*line;
	int		status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{ // Child process: read heredoc input.
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ; // End-of-file (Ctrl-D) or error.
			if (ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break ; // Delimiter reached.
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]); // Close write end in the child.
		exit(EXIT_SUCCESS);
	}
	else
	{                             // Parent process.
		close(pipefd[1]);         // Parent only needs the read end.
		waitpid(pid, &status, 0); // Wait for the child to finish.
		return (pipefd[0]);       // Return the read end of the pipe.
	}
}

/*
 * print_heredoc_contents:
 * Reads the entire content from the given file descriptor (fd)
 * and prints it to standard output.
 *
 * Warning: This will consume the contents from fd.
 */
void	print_heredoc_contents(int fd)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
	{
		write(STDOUT_FILENO, buffer, bytes_read);
	}
	if (bytes_read < 0)
	{
		perror("read");
	}
}

int	main(void)
{
	char	*delimiter;
	int		heredoc_fd;

	delimiter = "EOF";
	heredoc_fd = heredoc(delimiter);
	// Print the contents of the heredoc.
	printf("Heredoc contents:\n");
	print_heredoc_contents(heredoc_fd);
	// If you intend to use the heredoc as input for a cmd (via dup2),
	// you would need to capture and store the data before printing it,
	// because reading from the fd here consumes the input.
	close(heredoc_fd);
	return (0);
}
