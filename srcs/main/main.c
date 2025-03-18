/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/17 15:38:28 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Print a single command's information

void	print_command(t_command *command, int cmd_index)
{
	size_t	i;

	i = 0;
	printf("Command %d:\n", cmd_index);
	// Print arguments
	if (command->args)
	{
		while (command->args[i])
		{
			printf("  Arg %ld: %s\n", i, command->args[i]);
			i++;
		}
	}
	else
	{
		printf("  No arguments\n");
	}
	// Print redirections, if any
	if (command->input_file)
		printf("  Input redirection: %s\n", command->input_file);
	if (command->heredoc_count > 0)
	{
		i = 0;
		while (i < command->heredoc_count)
		{
			printf("heredoc_files[%ld]:%s\n", i, command->heredoc_files[i]);
			i++;
		}
	}
	if (command->out_file)
	{
		printf("  Output redirection: %s\n", command->out_file);
		printf("  Append mode: %s\n", command->is_append ? "Yes" : "No");
	}
	printf("\n");
}

// Print all commands in the linked list
void	print_commands(t_command *head)
{
	int			cmd_index;
	t_command	*current;

	cmd_index = 0;
	current = head;
	while (current)
	{
		print_command(current, cmd_index);
		current = current->next;
		cmd_index++;
	}
}

// Free a single command node (including its arguments and redirection strings)
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

// Free the entire linked list of commands
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

static t_minishell	*create_shell_struct(void)
{
	t_minishell	*new_shell;

	new_shell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!new_shell)
		return (NULL);
	new_shell->env = NULL;
	new_shell->tokens = NULL;
	new_shell->commands = NULL;
	new_shell->exit_status = 0;
	return (new_shell);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_minishell	*shell;

	if (argc > 1)
		printf("minishell: %s: No such file or directory\n", argv[1]);
	shell = create_shell_struct();
	shell->env = env_duplication(envp);
	setup_signals();
	while (1)
	{
		if (g_signal)
		{
			g_signal = 0;
			input = prompt();
			if (!input)
			{
				printf("exit\n");
				break ;
			}
		}
		else
			input = prompt();
		if (input)
		{
			if (!input[0])
				continue ;
			// printf("Input: %s\n", input);
			shell->tokens = lexer(input);
			if (shell->tokens)
				print_tokens(shell->tokens);
			shell->commands = parser(shell->tokens);
			if (shell->commands)
			{
				expand_commands(shell->commands, shell->env);
				print_commands(shell->commands);
				command_executor(shell);
			}
			free(input);
			free_tokens(shell->tokens);
			free_commands(shell->commands);
		}
		else
			break ;
	}
	return (0);
}
