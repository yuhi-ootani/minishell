/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:29:30 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 13:15:16 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// t_token_type	last_infile_type(t_command *cmd)
// {
// 	if (cmd->infile_count == 0)
// 		return (TOKEN_WORD);
// 	return (cmd->infiles[cmd->infile_count - 1].type);
// }

bool	input_redirection(t_minishell *shell, t_command *cmd)
{
	size_t	i;
	char	*filename;
	int		infile_fd;

	i = 0;
	while (i < cmd->infile_count)
	{
		filename = cmd->infiles[i].filename;
		infile_fd = open(filename, O_RDONLY);
		if (infile_fd < 0)
		{
			ft_fprintf(STDERR_FILENO, "MINISHELL: %s: %s\n", filename,
				strerror(errno));
			shell->exit_status = EXIT_FAILURE;
			return (false);
		}
		if (cmd->infile_count == i + 1)
			dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
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

bool	handle_redirection(t_minishell *shell, t_command *cmd)
{
	int	error;

	error = 0;
	if (cmd->infile_count > 0)
	{
		if (!input_redirection(shell, cmd))
			error++;
	}
	if (cmd->outfile_count > 0)
	{
		if (!output_redirection(shell, cmd))
			error++;
	}
	if (error > 0)
		return (false);
	return (true);
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