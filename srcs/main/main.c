/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/20 16:17:49 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_command(t_command *command)
{
	int	i;

	i = 0;
	if (command->args)
	{
		while (command->args[i])
		{
			free(command->args[i]);
			i++;
		}
		free(command->args);
	}
	if (command->input_file)
		free(command->input_file);
	if (command->out_file)
		free(command->out_file);
	free(command);
}

void	free_commands(t_command *head)
{
	t_command	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free_command(tmp);
	}
}
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

char	*get_input(int argc, char **argv, bool *interactive)
{
	static bool	initialized = false;
	char		*input_line;
	int			fd;

	if (initialized == false)
	{
		if (argc > 1)
		{
			fd = open(argv[1], O_RDONLY);
			if (fd == -1)
			{
				perror("Failed to open file");
				return (NULL); // Handle the error if file can't be opened
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("Failed to duplicate file descriptor");
				close(fd);
				return (NULL);
			}
			close(fd);
		}
		if (!isatty(STDIN_FILENO))
			*interactive = false;
		initialized = true;
	}
	if (!*interactive)
	{
		input_line = ft_get_next_line(STDIN_FILENO);
		if (input_line && strncmp(input_line, "#", 1) == 0)
		{
			free(input_line);
			input_line = ft_get_next_line(STDIN_FILENO);
		}
		return (input_line);
	}
	else
		return (prompt());
}

int	main(int argc, char **argv, char **envp)
{
	bool		interactive;
	char		*input;
	t_minishell	shell;

	interactive = true;
	init_shell_struct(&shell, envp);
	setup_signals();
	while (1)
	{
		if (g_signal)
			g_signal = 0;
		input = get_input(argc, argv, &interactive);
		if (!input)
		{
			printf("exit\n"); // todo
			break ;
		}
		if (input && input[0] != '\n') // to do
		{
			if (!input[0])
				continue ;
			shell.tokens = lexer(input);
			if (shell.tokens)
				print_tokens(shell.tokens);
			shell.commands = parser(shell.tokens);
			if (shell.commands)
			{
				expand_commands(&shell);
				print_commands(shell.commands);
				command_executor(&shell);
			}
			free_tokens(shell.tokens);
			shell.tokens = NULL;
			free_commands(shell.commands);
			shell.commands = NULL;
			if (interactive)
				dup2(shell.original_stdin, STDIN_FILENO);
			dup2(shell.original_stdout, STDOUT_FILENO);
		}
		else
			break ;
	}
	close(shell.original_stdin);
	close(shell.original_stdout);
	return (0);
}
