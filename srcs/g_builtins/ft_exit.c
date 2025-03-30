/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:02 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/29 16:27:17 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	return ((long long)(num * neg));
}

int	more_than_two_arguments(t_minishell *shell)
{
	t_command	*cmd;

	cmd = shell->commands;
	if (cmd->args[1] && cmd->args[2])
	{
		ft_fprintf(2, "exit: too many arguments\n");
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

long long	validate_and_exit_if_invalid(t_minishell *shell)
{
	t_command	*cmd;
	char		*arg;
	bool		error;
	long long	exit_code;

	cmd = shell->commands;
	arg = cmd->args[1];
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
	return (exit_code);
}

void	ft_exit(t_minishell *shell)
{
	int			exit_code;
	t_command	*cmd;

	cmd = shell->commands;
	printf("exit\n");
	if (more_than_two_arguments(shell))
		return ;
	if (!cmd->args[1])
	{
		exit_code = shell->exit_status;
	}
	else
		exit_code = validate_and_exit_if_invalid(shell);
	shell->exit_status = exit_code % 256;
	free_shell(shell);
	exit(exit_code);
}
