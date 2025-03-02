/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:02 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/01 16:10:43 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(t_command *command)
{
	int	exit_code;

	exit_code = 0;
	printf("exit\n");
	if (command->args[1] && command->args[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		return (1);
	}
	if (command->args[1])
	{
		if (!ft_isnumber(command->args[1]))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n",
				command->args[1]);
			exit(2);
		}
		exit_code = ft_atoi(command->args[1]);
	}
	exit(exit_code % 256);
}
