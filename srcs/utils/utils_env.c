/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   touch utils_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:47:09 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 14:56:56 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*create_new_env_util(const char *new_name, const char *new_value,
		t_env *new_next)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->name = ft_strdup(new_name);
	if (!new_env->name)
	{
		free(new_env);
		return (NULL);
	}
	if (new_value)
	{
		new_env->value = ft_strdup(new_value);
		if (!new_env->value)
		{
			free(new_env->name);
			free(new_env);
			return (NULL);
		}
	}
	else
		new_env->value = NULL;
	new_env->next = new_next;
	return (new_env);
}

void	env_add_back_util(t_env **copied_env, t_env *new_env)
{
	t_env	*tmp;

	tmp = *copied_env;
	if (!tmp)
	{
		*copied_env = new_env;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env;
}

bool	get_env_value(t_minishell *shell, const char *name, char **result)
{
	t_env	*env;

	env = shell->env;
	if (!env || !name)
		return (result);
	*result = NULL;
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			if (env->value == NULL)
				return (true);
			*result = ft_strdup(env->value);
			if (!*result)
				return (false);
			return (true);
		}
		env = env->next;
	}
	return (true);
}
