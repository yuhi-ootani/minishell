/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/22 10:28:49 by oyuhi            ###   ########.fr       */
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

void	free_shell(t_minishell *shell)
{
	close(shell->original_stdout);
	close(shell->original_stdin);
	free_copied_env(shell->env);
	if (shell->input)
		free(shell->input);
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

void	reset_shell_for_next_input(t_minishell *shell, bool interactive_mode)
{
	// Reset errno before reading input
	errno = 0;
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->commands)
	{
		free_commands(shell->commands);
		shell->commands = NULL;
	}
	if (interactive_mode)
		dup2(shell->original_stdin, STDIN_FILENO);
	dup2(shell->original_stdout, STDOUT_FILENO);
}

void	build_commands_struct(t_minishell *shell)
{
	t_token	*tokens;

	tokens = lexer(shell->input);
	if (!tokens)
		return ;
	print_tokens(tokens);
	shell->commands = parser(tokens);
	if (!shell->commands)
		return ;
	expand_commands(shell);
	print_commands(shell->commands);
}

int	main(int argc, char **argv, char **envp)
{
	bool		interactive_mode;
	t_minishell	shell;

	init_shell_struct(&shell, envp);
	interactive_mode = decide_input_fd(&shell, argc, argv);
	setup_signals();
	while (1)
	{
		// signal 130
		if (g_signal)
		{
			g_signal = 0;
			shell.exit_status = 130;
		}
		shell.input = get_input(&shell, interactive_mode);
		if (!shell.input)
		{
			if (errno == 0 && !interactive_mode)
				break ;
			else if (errno == 0 && !g_signal)
			{
				printf("fsdfsdfexit\n");
				break ;
			}
		}
		else // todo signal
		{
			if (shell.input && shell.input[0] && shell.input[0] != '\n')
				// to do
				build_commands_struct(&shell);
			if (shell.commands)
				command_executor(&shell);
		}
		reset_shell_for_next_input(&shell, interactive_mode);
	}
	free_shell(&shell);
	return (shell.exit_status);
}
