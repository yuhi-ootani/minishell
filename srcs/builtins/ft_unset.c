/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/24 17:28:49 by oyuhi            ###   ########.fr       */
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
	t_command	*cmd;

	i = 1;
	cmd = shell->commands;
	while (cmd->args[i])
	{
		var_name = cmd->args[i];
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
