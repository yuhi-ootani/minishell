/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/10 15:20:21 by knemcova         ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens_list;
	t_command	*command_list;

	(void)argc;
	(void)argv;
	(void)envp;
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
			printf("Input: %s\n", input);
			tokens_list = lexer(input);
			if (tokens_list)
				print_tokens(tokens_list);
			command_list = parser(tokens_list);
			if (command_list)
			{
				expand_commands(command_list);
				print_commands(command_list);
				command_executor(command_list, envp);
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
