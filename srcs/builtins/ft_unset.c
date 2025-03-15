/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/15 16:41:52 by oyuhi            ###   ########.fr       */
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

static void	remove_target_env(t_env **copied_env, t_env *prev_env,
		t_env *target_env)
{
	if (prev_env)
		prev_env->next = target_env->next;
	else
		*copied_env = target_env->next;
	free(target_env->name);
	free(target_env->value);
	free(target_env);
}

void	ft_unset(t_minishell *shell)
{
	size_t		i;
	char		*var_name;
	t_env		*prev_env;
	t_env		*tmp_env;
	t_command	*command;

	i = 1;
	command = shell->commands;
	while (command->args[i])
	{
		var_name = command->args[i];
		prev_env = NULL;
		tmp_env = shell->env;
		while (tmp_env)
		{
			if (ft_strcmp(tmp_env->name, var_name) == 0)
				remove_target_env(&shell->env, prev_env, tmp_env);
			prev_env = tmp_env;
			tmp_env = tmp_env->next;
		}
		i++;
	}
}
