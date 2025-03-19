/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:02 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/19 13:25:55 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exit(t_minishell *shell)
{
	int			exit_code;
	t_command	*command;

	exit_code = 0;
	printf("exit\n");
	command = shell->commands;
	if (command->args[1] && command->args[2])
	{
		ft_fprintf(2, "exit: too many arguments\n");
		exit(EXIT_FAILURE);
	}
	if (command->args[1])
	{
		if (!ft_isnumber(command->args[1]))
		{
			ft_fprintf(2, "exit: %s: numeric argument required\n",
				command->args[1]);
			exit(2); // todo
		}
		exit_code = ft_atoi(command->args[1]);
	}
	exit(exit_code % 256);
}

// void	handle_exit_status(t_minishell *shell, int exit_code,
// 		const char *error_message)
// {
// 	if (error_message)
// 		ft_fprintf(2, "%s\n", error_message);
// 	if (shell->env)
// 		free_env(shell->env);
// 	if (shell->tokens)
// 		free_tokens(shell->tokens);
// 	if (shell->commands)
// 		free_commands(shell->commands);
// 	*(shell->exit_status) = exit_code;
// 	exit(exit_code);
// }

// void	ft_exit(t_minishell *shell)
// {
// 	int			exit_code;
// 	t_command	*command;

// 	exit_code = 0;
// 	printf("exit\n");
// 	command = shell->commands;

// 	if (command->args[1] && command->args[2])
// 		handle_exit_status(shell, 1, "exit: too many arguments");

// 	if (command->args[1])
// 	{
// 		if (!ft_isnumber(command->args[1]))
// 			handle_exit_status(shell, 2, "exit: numeric argument required");

// 		exit_code = ft_atoi(command->args[1]);
// 	}

// 	handle_exit_status(shell, exit_code, NULL);
// }
