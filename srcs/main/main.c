/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/21 18:22:35 by knemcova         ###   ########.fr       */
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

char	*get_input(bool interactive_mode)
{
	char	*input_line;

	if (!interactive_mode)
	{
		input_line = ft_get_next_line(STDIN_FILENO);
		if (input_line && ft_strncmp(input_line, "#", 1) == 0)
		{
			free(input_line);
			input_line = ft_get_next_line(STDIN_FILENO);
		}
		return (input_line);
	}
	else
		return (prompt());
}

void	free_shell(t_minishell *shell)
{
	printf("stdin:%d, stdout:%d\n", shell->original_stdin,
		shell->original_stdout);
	close(shell->original_stdout);
	close(shell->original_stdin);
	free_copied_env(shell->env);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
}

int	get_exit_status(int err) // Fix: Use a proper function parameter
{
	if (err == ENOENT)
		return (127);
	else if (err == EACCES)
		return (126);
	else
		return (1);
}

bool	decide_input_fd(t_minishell *shell, int argc, char **argv)
{
	int	fd;

	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			free_shell(shell);
			shell->exit_status = get_exit_status(errno);
			ft_fprintf(2, "MINISHELL: %s: %s\n", argv[1], strerror(errno));
			exit(shell->exit_status);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			free_shell(shell);
			exit(FAIL_DUP);
		}
		close(fd);
		return (false);
	}
	else if (!isatty(STDIN_FILENO))
		return (false);
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	bool		interactive_mode;
	char		*input;
	t_minishell	shell;

	init_shell_struct(&shell, envp);
	interactive_mode = decide_input_fd(&shell, argc, argv);
	setup_signals();
	while (1)
	{
		if (g_signal)
			g_signal = 0;
		input = get_input(interactive_mode);
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
			if (interactive_mode)
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
