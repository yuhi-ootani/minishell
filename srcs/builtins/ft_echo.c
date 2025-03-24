/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/24 18:33:42 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_n_option(t_command *command, int *n_option)
{
	int	i;
	int	j;

	i = 1;
	*n_option = 0;
	while (command->args[i] && command->args[i][0] == '-')
	{
		j = 1;
		while (command->args[i][j] && command->args[i][j] == 'n')
			j++;
		if (command->args[i][j] == '\0')
		{
			*n_option = 1;
			i++;
		}
		else
			break ;
	}
	return (i);
}

void	print_echo_output(t_command *command, int start_index, int n_option)
{
	while (command->args[start_index])
	{
		ft_putstr_fd(command->args[start_index], 1);
		if (command->args[start_index + 1])
			write(1, " ", 1);
		start_index++;
	}
	if (!n_option)
		write(1, "\n", 1);
}

void	ft_echo(t_minishell *shell)
{
	int			n_option;
	int			start_index;
	t_command	*command;

	command = shell->commands;
	start_index = parse_n_option(command, &n_option);
	print_echo_output(command, start_index, n_option);
}
