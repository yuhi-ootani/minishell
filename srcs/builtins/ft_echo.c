/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/02 10:17:59 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// static int	counts_arguments(t_command *command)
// {
// 	int	size;

// 	size = 0;
// 	while (command->args[size])
// 		size++;
// 	return (size);
// }

// static void	print_argument(char *arg)
// {
// 	int		single_quotes;
// 	int		double_quotes;
// 	char	*env_value;

// 	single_quotes = (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'');
// 	double_quotes = (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"');
// 	if (double_quotes)
// 	{
// 		arg[ft_strlen(arg) - 1] = '\0';
// 		arg++;
// 	}
// 	if (!single_quotes && arg[0] == '$')
// 	{
// 		env_value = getenv(arg + 1);
// 		if (env_value)
// 			ft_putstr_fd(env_value, 1);
// 	}
// 	else
// 	{
// 		if (single_quotes)
// 		{
// 			arg[ft_strlen(arg) - 1] = '\0';
// 			ft_putstr_fd(arg + 1, 1);
// 		}
// 		else
// 			ft_putstr_fd(arg, 1);
// 	}
// }

// int	ft_echo(t_command *command)
// {
// 	int	i;
// 	int	n_option;

// 	i = 1;
// 	n_option = 0;
// 	if (counts_arguments(command) > 1)
// 	{
// 		while (command->args[i] && ft_strcmp(command->args[i], "-n") == 0)
// 		{
// 			n_option = 1;
// 			i++;
// 		}
// 		while (command->args[i])
// 		{
// 			print_argument(command->args[i]);
// 			if (command->args[i + 1])
// 				write(1, " ", 1);
// 			i++;
// 		}
// 	}
// 	if (n_option == 0)
// 		write(1, "\n", 1);
// 	return (0);
// }

static int	is_option_n(char *arg)
{
	int	j;

	if (!arg || arg[0] != '-')
		return (0);
	j = 1;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (0);
		j++;
	}
	return (1);
}

static void	print_argument(char *arg)
{
	int		single_quotes;
	int		double_quotes;
	char	*env_value;

	single_quotes = (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'');
	double_quotes = (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"');
	if (double_quotes)
	{
		arg[ft_strlen(arg) - 1] = '\0';
		arg++;
		if (arg[0] == '$')
		{
			if (arg[1] == '\0')
				ft_putstr_fd("$", 1);
			else
			{
				env_value = getenv(arg + 1);
				if (env_value)
					ft_putstr_fd(env_value, 1);
				else
					ft_putstr_fd("", 1);
			}
		}
		else
			ft_putstr_fd(arg, 1);
	}
	else if (single_quotes)
	{
		arg[ft_strlen(arg) - 1] = '\0';
		ft_putstr_fd(arg + 1, 1);
	}
	else
	{
		if (arg[0] == '$')
		{
			if (arg[1] == '\0')
				ft_putstr_fd("$", 1);
			else
			{
				env_value = getenv(arg + 1);
				if (env_value)
					ft_putstr_fd(env_value, 1);
				else
					ft_putstr_fd("", 1);
			}
		}
		else
			ft_putstr_fd(arg, 1);
	}
}

int	ft_echo(t_command *command)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	while (command->args[i] && is_option_n(command->args[i]))
	{
		n_option = 1;
		i++;
	}
	while (command->args[i])
	{
		print_argument(command->args[i]);
		if (command->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}
