/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:16:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/02/26 17:43:36 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_command *command)
{
	char	*path;

	if (!command->args[1])
	{
		path = getenv("HOME");
		if (!path || *path == '\0')
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = command->args[1];
	if (chdir(path) != 0)
	{
		fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}

int	ft_export(t_command *command)
{
	char	*var;
	char	*value;
	int		i;

	if (!command->args[1] || ft_strchr(command->args[1], '=') == NULL)
	{
		fprintf(stderr, "export: invalid format. Use VAR=value\n");
		return (1);
	}
	i = 0;
	while (command->args[1][i] && command->args[1][i] != '=')
		i++;
	if (i > 0)
	{
		var = ft_strndup(command->args[1], i);
		value = ft_strdup(command->args[1] + i + 1);
	}
	else
	{
		fprintf(stderr, "export: invalid format. Use VAR=value\n");
		return (1);
	}
	if (setenv(var, value, 1) != 0)
		perror("export");
	free(var);
	free(value);
	return (0);
}

int	ft_unset(t_command *command)
{
	if (!command->args[1])
	{
		fprintf(stderr, "unset: argument not found\n");
		return (1);
	}
	if (unsetenv(command->args[1]) != 0)
	{
		perror("unset");
		return (1);
	}
	return (0);
}

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
