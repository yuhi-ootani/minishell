/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/15 09:15:12 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_echo(t_command *command, t_env **copied_env)
{
	int	i;
	int	n_option;
	int	j;

	(void)copied_env;
	i = 1;
	n_option = 0;
	while (command->args[i] && command->args[i][0] == '-')
	{
		j = 1;
		while (command->args[i][j] && command->args[i][j] == 'n')
			j++;
		if (command->args[i][j] == '\0')
		{
			n_option = 1;
			i++;
		}
		else
			break ;
	}
	while (command->args[i])
	{
		ft_putstr_fd(command->args[i], 1);
		if (command->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_option)
		write(1, "\n", 1);
}

