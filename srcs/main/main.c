/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/19 17:31:20 by oyuhi            ###   ########.fr       */
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

// input = prompt();
// if (!input)
// {
// 	printf("exit\n");
// 	break ;
// }
// if (*input)
// 	add_history(input);
// tokens_list = lexer(input);
// command_list = parser(tokens_list);
// if (command_list)
// {
// 	if (is_builtin(command_list->args[0]) == FT_EXIT)
// 	{
// 		exit_code = ft_exit(command_list);
// 		break ;
// 	}
// 	single_command_executor(command_list, envp);
// }
// free(input);
// free_tokens(tokens_list);
// free_commands(command_list);
// }
// return (exit_code);
// }

char	*get_input(void)
{
	static int	interactive = -1;
	char		*input_line;

	if (interactive == -1)
		interactive = isatty(STDIN_FILENO);
	if (interactive == false)
	{
		input_line = ft_get_next_line(STDIN_FILENO);
		if (input_line)
			return (input_line);
		else
			return (NULL);
	}
	return (prompt());
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
	int			original_stdin;
	int			original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (argc > 1)
		printf("minishell: %s: No such file or directory\n", argv[1]);
	shell = create_shell_struct();
	shell->env = env_duplication(envp);
	setup_signals();
	while (1)
	{
		if (g_signal)
			g_signal = 0;
		input = get_input();
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input && input[0] != '\n') // to do
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
				expand_commands(shell);
				print_commands(shell->commands);
				command_executor(shell);
			}
			// free_token_list(shell->tokens);
			// shell->tokens = NULL;
			// free_command_list(shell->commands);
			// shell->commands = NULL;
			dup2(original_stdin, STDIN_FILENO);
			dup2(original_stdout, STDOUT_FILENO);
		}
		else
			break ;
	}
	close(original_stdin);
	return (0);
}
