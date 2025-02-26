/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:16:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/02/26 14:43:28 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_commands(char **args, char **envp)
{
	if (ft_strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(args[0], "export") == 0)
		ft_export(args);
	else if (ft_strcmp(args[0], "unset") == 0)
		ft_unset(args[1]);
	else if (ft_strcmp(args[0], "env") == 0)
		ft_env(envp);
	else if (ft_strcmp(args[0], "exit") == 0)
		ft_exit(args);
	if (ft_strcmp(args[0], "cd") == 0)
	{
		ft_cd(args[1]);
		return (1);
	}
	return (0);
}

void	ft_cd(char *path)
{
	char	*home;

	if (!path || *path == '\0')
	{
		home = getenv("HOME");
		if (!home || *home == '\0')
		{
			fprintf(stderr, "cd: HOME not set\n");
			return ;
		}
		path = home;
	}
	if (chdir(path) != 0)
	{
		fprintf(stderr, "cd: %s: %s\n", strerror(errno), path);
	}
}
// ok
/*the info about working directory is allocated in a place called PBC,
from which the function getcwd can read the info a chdir can change it.*/

void	ft_export(char **args)
{
	char	*var;
	char	*value;
	int		i;

	if (!args[1] || ft_strchr(args[1], '=') == NULL)
	{
		fprintf(stderr, "export: invalid format. Use VAR=value\n");
		return ;
	}
	i = 0;
	while (args[1][i] && args[1][i] != '=')
		i++;
	if (i > 0)
	{
		var = ft_strndup(args[1], i);
		value = ft_strdup(args[1] + i + 1);
	}
	else
	{
		fprintf(stderr, "export: invalid format. Use VAR=value\n");
		return ;
	}
	if (setenv(var, value, 1) != 0)
		perror("export");
	free(var);
	free(value);
}

void	ft_unset(char *var)
{
	if (!var)
	{
		fprintf(stderr, "unset: argument not found\n");
		return ;
	}
	if (unsetenv(var) != 0)
		perror("unset");
}

void	ft_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	if (args[1] && args[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		return ;
	}
	if (args[1])
	{
		if (!ft_isnumber(args[1]))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			exit(2);
		}
		exit_code = ft_atoi(args[1]);
	}
	exit(exit_code % 256);
}
