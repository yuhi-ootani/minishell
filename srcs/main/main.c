/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/29 15:13:23 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_command(t_command *cmd)
{
	size_t	i;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	// t_redirection
	i = 0;
	if (cmd->infiles)
	{
		while (i < cmd->infile_count)
		{
			free(cmd->infiles[i].filename);
			i++;
		}
		free(cmd->infiles);
	}
	i = 0;
	if (cmd->outfiles)
	{
		while (i < cmd->outfile_count)
		{
			free(cmd->outfiles[i].filename);
			i++;
		}
		free(cmd->outfiles);
	}
	// if (cmd->input_file)
	// 	free(cmd->input_file);
	// if (cmd->out_file)
	// 	free(cmd->out_file);
	free(cmd);
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
	errno = 0;
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
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

	tokens = tokenizer(shell);
	if (!tokens)
		return ;
	print_tokens(tokens);
	shell->commands = parser(shell, tokens);
	free_tokens(tokens);
	if (!shell->commands)
	{
		return ;
	}
	expand_all_cmd_args(shell);
	print_commands(shell->commands);
}

void	exit_ctrl_D(t_minishell *shell)
{
	free_shell(shell);
	printf("exit\n");
	exit(shell->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	bool		interactive_mode;
	t_minishell	shell;

	init_shell_struct(&shell, envp);
	interactive_mode = decide_input_fd(&shell, argc, argv);
	setup_signals_parent();
	while (1)
	{
		if (g_signal)
			g_signal = 0;
		shell.input = get_input(&shell, interactive_mode);
		if (!shell.input)
			exit_ctrl_D(&shell);
		else
		{
			if (shell.input && shell.input[0] && shell.input[0] != '\n')
				build_commands_struct(&shell);
			if (shell.commands)
				command_executor(&shell);
		}
		reset_shell_for_next_input(&shell, interactive_mode);
	}
	free_shell(&shell);
	return (shell.exit_status);
}
