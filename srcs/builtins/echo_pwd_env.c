/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pwd_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/01 09:48:12 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	ft_env(char **envp)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		printf("%s\n", envp[i]);
// 		i++;
// 	}
// }

static int	counts_arguments(t_command *command)
{
	int	size;

	size = 0;
	while (command->args[size])
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


int	ft_echo(t_command *command)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	if (counts_arguments(command) > 1)
	{
		while (command->args[i] && ft_strcmp(command->args[i], "-n") == 0)
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
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}

int	ft_pwd(t_command *command)
{
	char	cwd[PATH_MAX];

	(void)command;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	ft_env(t_command *command)
{
	extern char	**environ;
	int			i;

	(void)command;
	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}
