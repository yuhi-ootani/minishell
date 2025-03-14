/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/14 13:20:46 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	ft_unset(t_command *command, t_env *copied_env)
// {
// 	(void)copied_env;
// 	if (!command->args[1])
// 	{
// 		fprintf(stderr, "unset: argument not found\n");
// 		return (1);
// 	}
// 	if (unsetenv(command->args[1]) != 0)
// 	{
// 		perror("unset");
// 		return (1);
// 	}
// 	return (0);
// }

static void	remove_target_env(t_env **copied_env, t_env *prev, t_env *tmp)
{
	if (prev)
		prev->next = tmp->next;
	else
		*copied_env = tmp->next;
	free(tmp->name);
	free(tmp->value);
	free(tmp);
}

void	ft_unset(t_command *command, t_env **copied_env)
{
	size_t	i;
	char	*var_name;
	t_env	*prev;
	t_env	*tmp;

	i = 1;
	while (command->args[i])
	{
		var_name = command->args[i];
		prev = NULL;
		tmp = *copied_env;
		while (tmp)
		{
			if (ft_strcmp(tmp->name, var_name) == 0)
				remove_target_env(copied_env, prev, tmp);
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
}
