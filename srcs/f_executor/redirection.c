/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:29:30 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 14:58:38 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// t_token_type	last_infile_type(t_command *cmd)
// {
// 	if (cmd->infile_count == 0)
// 		return (TOKEN_WORD);
// 	return (cmd->infiles[cmd->infile_count - 1].type);
// }

static void	print_error_message(t_minishell *shell, char *filename)
{
	ft_fprintf(STDERR_FILENO, "MINISHELL:%s: %s\n", filename, strerror(errno));
	set_exit_failure(shell);
	if (filename)
		free(filename);
}

bool	input_redirection(t_minishell *shell, t_command *cmd)
{
	size_t	i;
	char	*filename;
	int		infile_fd;

	i = 0;
	while (i < cmd->infile_count)
	{
		filename = expand_filename(shell, cmd->infiles[i]);
		if (!filename)
			return (false);
		infile_fd = open(filename, O_RDONLY);
		if (infile_fd < 0)
			return (print_error_message(shell, filename), false);
		free(filename);
		if (cmd->infile_count == i + 1)
		{
			if (dup2(infile_fd, STDIN_FILENO) == -1)
				return (set_exit_failure(shell), close(infile_fd), false);
		}
		close(infile_fd);
		i++;
	}
	return (true);
}

// t_token_type	last_outfile_type(t_command *cmd)
// {
// 	if (cmd->outfile_count == 0)
// 		return (TOKEN_WORD);
// 	return (cmd->outfiles[cmd->outfile_count - 1].type);
// }

int	open_outfile(t_command *cmd, size_t i, char *filename)
{
	int	flags;
	int	outfile_fd;

	outfile_fd = -1;
	flags = O_WRONLY | O_CREAT;
	if (cmd->outfiles[i].type == TOKEN_APPEND)
		flags |= O_APPEND;
	else if (cmd->outfiles[i].type == TOKEN_REDIR_OUT)
		flags |= O_TRUNC;
	outfile_fd = open(filename, flags, 0644);
	return (outfile_fd);
}

bool	output_redirection(t_minishell *shell, t_command *cmd)
{
	int		outfile_fd;
	size_t	i;
	char	*filename;

	i = 0;
	while (i < cmd->outfile_count)
	{
		filename = expand_filename(shell, cmd->outfiles[i]);
		if (!filename)
			return (false);
		outfile_fd = open_outfile(cmd, i, filename);
		if (outfile_fd < 0)
			return (print_error_message(shell, filename), false);
		free(filename);
		if (i + 1 == cmd->outfile_count)
		{
			if (dup2(outfile_fd, STDOUT_FILENO) == -1)
				return (set_exit_failure(shell), close(outfile_fd), false);
		}
		close(outfile_fd);
		i++;
	}
	return (true);
}

// bool	handle_redirection(t_minishell *shell, t_command *cmd)
// {
// 	int	error;

// 	error = 0;
// 	if (cmd->infile_count > 0)
// 	{
// 		if (!input_redirection(shell, cmd))
// 			error++;
// 	}
// 	if (cmd->outfile_count > 0)
// 	{
// 		if (!output_redirection(shell, cmd))
// 			error++;
// 	}
// 	if (error > 0)
// 		return (false);
// 	return (true);
// }

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