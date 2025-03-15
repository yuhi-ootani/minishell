/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:02 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/14 13:19:54 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exit(t_command *command, t_env **copied_env)
{
	int	exit_code;

	exit_code = 0;
	printf("exit\n");
	(void)copied_env;
	if (command->args[1] && command->args[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		exit(EXIT_FAILURE);
	}
	if (command->args[1])
	{
		if (!ft_isnumber(command->args[1]))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n",
				command->args[1]);
			exit(2); // todo
		}
		exit_code = ft_atoi(command->args[1]);
	}
	exit(exit_code % 256);
}
