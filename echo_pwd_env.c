/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pwd_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/02/24 16:08:01 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	counts_arguments(char **args)
{
	int	size;

	size = 0;
	while (args[size])
		size++;
	return (size);
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
	}
	if (!single_quotes && arg[0] == '$')
	{
		env_value = getenv(arg + 1);
		if (env_value)
			ft_putstr_fd(env_value, 1);
	}
	else
	{
		if (single_quotes)
		{
			arg[ft_strlen(arg) - 1] = '\0';
			ft_putstr_fd(arg + 1, 1);
		}
		else
			ft_putstr_fd(arg, 1);
	}
}

int	ft_echo(char **args)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	if (counts_arguments(args) > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (args[i])
		{
			print_argument(args[i]);
			if (args[i + 1])
				write(1, " ", 1);
			i++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	else
		return (1);
}

void	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
// its okay now but maybe it will be hard to change values