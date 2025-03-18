/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/18 19:22:28 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
