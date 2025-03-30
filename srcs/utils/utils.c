/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:17:05 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/29 15:24:59 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*remove_quotes(t_minishell *shell, const char *input)
{
	char	*result;

	result = (char *)ft_calloc(sizeof(char), ft_strlen(input) + 1);
	if (!result)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	remove_quotes_and_copy(result, input);
	return (result);
}

size_t	count_env_util(t_env *env)
{
	size_t	count;

	count = 0;
	if (!env)
		return (count);
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

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

char	*get_env_value(t_minishell *shell, const char *name)
{
	char	*copied_value;
	t_env	*env;

	env = shell->env;
	if (!env || !name)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			if (env->value == NULL)
				return (NULL);
			copied_value = ft_strdup(env->value);
			if (!copied_value)
			{
				shell->exit_status = EXIT_FAILURE;
				return (NULL);
			}
			return (copied_value);
		}
		env = env->next;
	}
	return (NULL);
}
