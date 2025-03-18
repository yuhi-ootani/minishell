/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:02 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/18 14:43:00 by knemcova         ###   ########.fr       */
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
