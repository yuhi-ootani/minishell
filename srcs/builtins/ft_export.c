/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:45:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/01 15:45:31 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
