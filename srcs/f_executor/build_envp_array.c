/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_envp_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:12:38 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 10:51:25 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**build_envp_array(t_minishell *shell)
{
	size_t	count;
	char	**envp_array;
	t_env	*tmp;

	count = 0;
	count = count_env_util(shell->env);
	envp_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp_array)
		cleanup_and_exit_failure(shell, NULL);
	count = 0;
	tmp = shell->env;
	while (tmp)
	{
		envp_array[count] = ft_strjoin_three(tmp->name, "=", tmp->value);
		if (!envp_array[count])
			cleanup_and_exit_failure(shell, NULL);
		count++;
		tmp = tmp->next;
	}
	envp_array[count] = NULL;
	return (envp_array);
}

