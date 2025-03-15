/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:42:21 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/15 16:30:21 by oyuhi            ###   ########.fr       */
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

void	ft_env(t_minishell *shell)
{
	t_env	*tmp_env;

	tmp_env = shell->env;
	while (tmp_env)
	{
		if (tmp_env->value)
			printf("%s=%s\n", tmp_env->name, tmp_env->value);
		tmp_env = tmp_env->next;
	}
}
