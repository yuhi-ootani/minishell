/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:17:05 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 19:09:12 by knemcova         ###   ########.fr       */
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

void	strcpy_except_quotes(char *dst, const char *src)
{
	bool	in_single;
	bool	in_double;
	size_t	i;

	i = 0;
	in_single = false;
	in_double = false;
	while (*src)
	{
		if (*src == '\'' && !in_double)
			in_single = !in_single;
		else if (*src == '"' && !in_single)
			in_double = !in_double;
		else
		{
			dst[i] = *src;
			i++;
		}
		src++;
	}
	dst[i] = '\0';
}

char	*strdup_except_quotes_util(const char *input)
{
	char	*result;

	result = (char *)ft_calloc(sizeof(char), ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	strcpy_except_quotes(result, input);
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

void	set_exit_failure(t_minishell *shell)
{
	shell->exit_status = EXIT_FAILURE;
}
