/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/28 16:47:14 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_n_option(t_command *cmd, int *n_option)
{
	int	i;
	int	j;

	i = 1;
	*n_option = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] != '-')
			break ;
		j = 1;
		if (!cmd->args[i][j])
			break ;
		while (cmd->args[i][j] && cmd->args[i][j] == 'n')
			j++;
		if (j > 1 && cmd->args[i][j] == '\0')
		{
			*n_option = 1;
			i++;
		}
		else
			break ;
	}
	return (i);
}

void	print_echo_output(t_command *cmd, int start_index, int n_option)
{
	while (cmd->args[start_index])
	{
		ft_putstr_fd(cmd->args[start_index], 1);
		if (cmd->args[start_index + 1])
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
	t_command	*cmd;

	cmd = shell->commands;
	start_index = parse_n_option(cmd, &n_option);
	print_echo_output(cmd, start_index, n_option);
}
