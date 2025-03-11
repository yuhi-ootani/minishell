/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/11 17:19:58 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_unset(t_command *command, t_env *coppied_env)
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
