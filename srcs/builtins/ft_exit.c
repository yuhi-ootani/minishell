/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:02 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/21 12:21:55 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f')
		return (c);
	return (0);
}
static bool	check_out_of_range(int neg, unsigned long long num, bool *error)
{
	unsigned long long	max_neg;

	max_neg = ((unsigned long long)(-(LONG_MIN + 1)) + 1);
	if ((neg == 1 && num > (unsigned long long)LONG_MAX) || (neg == -1
			&& num > max_neg))
	{
		*error = true;
	}
	return (*error);
}
int	ft_atoi_long(const char *str, bool *error)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	*error = false;
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	neg = 1;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		if (check_out_of_range(neg, num, error))
			break ;
		i++;
	}
	return ((int)(num * neg));
}
int	more_than_two_arguments(t_minishell *shell)
{
	t_command	*command;

	command = shell->commands;
	if (command->args[1] && command->args[2])
	{
		ft_fprintf(2, "exit: too many arguments\n");
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

void	check_number(t_minishell *shell)
{
	t_command	*command;
	char		*arg;
	bool		error;
	long long	exit_code;

	command = shell->commands;
	arg = command->args[1];
	if (!ft_isnumber(arg))
	{
		ft_fprintf(2, "exit: %s: numeric argument required\n", arg);
		shell->exit_status = 2;
		free_shell(shell);
		exit(shell->exit_status);
	}
	exit_code = ft_atoi_long(arg, &error);
	if (error)
	{
		ft_fprintf(2, "exit: %s: numeric argument required\n", arg);
		shell->exit_status = 2;
		free_shell(shell);
		exit(shell->exit_status);
	}
	shell->exit_status = exit_code % 256;
	free_shell(shell);
	exit(shell->exit_status);
}

void	ft_exit(t_minishell *shell)
{
	int			exit_code;
	t_command	*command;

	command = shell->commands;
	printf("exit\n");
	if (more_than_two_arguments(shell))
		return ;
	if (!command->args[1])
	{
		exit_code = shell->exit_status;
	}
	else
	{
		check_number(shell);
		exit_code = (ft_atoi_long(command->args[1], &(bool){false}) % 256);
		shell->exit_status = exit_code;
	}
	free_shell(shell);
	exit(exit_code);
}
