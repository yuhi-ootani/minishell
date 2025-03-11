/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:45:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/11 17:37:00 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	set_new_env_variables(t_command *command, t_env *copied_env)
{
	char	*var;
	char	*value;
	size_t	i;

	if (ft_strchr(command->args[1], '=') == NULL)
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
}

int	ft_export(t_command *command, t_env *copied_env)
{
	if (command->args[1])
		set_new_env_variables(command, copied_env);
	else
		print_sorted_env(copied_env);
	return (0);
}
