/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/04 20:10:09 by oyuhi            ###   ########.fr       */
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

// int	main(void)
// {
// 	char		*input;
// 	t_token		*tokens_list;
// 	t_command	*command_list;

// 	printf("Enter commands:\n");
// 	printf("  'clear'   : Clear command history.\n");
// 	printf("  'replace' : Replace current line with a preset message.\n");
// 	printf("  'exit'    : Exit the program.\n\n");
// 	while (1)
// 	{
// 		input = prompt();
// 		if (input)
// 		{
// 			printf("Input: %s\n", input);
// 			tokens_list = lexer(input);
// 			free(input);
// 			if (tokens_list)
// 				print_tokens(tokens_list);
// 			command_list = parser(tokens_list);
// 			free_tokens(tokens_list);
// 			if (command_list)
// 			{
// 				print_commands(command_list);
// 				free_commands(command_list);
// 			}
// 		}
// 		else
// 			break ;
// 	}
// 	return (0);
// }

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens_list;
	t_command	*command_list;
	t_env		*copied_env;

	(void)argc;
	(void)argv;
	(void)envp;
	setup_signals();
	copied_env = env_duplication(envp);
	// printf("Enter commands:\n");
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
			printf("Input: %s\n", input);
			tokens_list = lexer(input);
			// if (tokens_list)
			// 	print_tokens(tokens_list);
			command_list = parser(tokens_list);
			if (command_list)
			{
				print_commands(command_list);
				command_executor(command_list, build_envp_array(copied_env));
			}
			free(input);
			free_tokens(tokens_list);
			free_commands(command_list);
		}
		else
			break ;
	}
	return (0);
}
