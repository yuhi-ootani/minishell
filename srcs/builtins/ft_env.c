/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:42:21 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/18 19:21:10 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
