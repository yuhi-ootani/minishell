/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:17:05 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 19:04:40 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_exit_failure(t_minishell *shell)
{
	shell->exit_status = EXIT_FAILURE;
}

// ▗▄▄▖ ▗▄▄▖ ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖    ▗▄▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▄▖ ▗▄▄▖
// ▐▌ ▐▌▐▌ ▐▌  █  ▐▛▚▖▐▌  █      ▐▌   ▐▌ ▐▌▐▛▚▖▐▌▐▌   ▐▌
// ▐▛▀▘ ▐▛▀▚▖  █  ▐▌ ▝▜▌  █      ▐▛▀▀▘▐▌ ▐▌▐▌ ▝▜▌▐▌    ▝▀▚▖
// ▐▌   ▐▌ ▐▌▗▄█▄▖▐▌  ▐▌  █      ▐▌   ▝▚▄▞▘▐▌  ▐▌▝▚▄▄▖▗▄▄▞▘

void	print_cmd(t_command *cmd, int cmd_index)
{
	size_t	i;
	char	*redir_type;

	printf("cmd %d:\n", cmd_index);
	// Print arguments.
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("  Arg %zu: %s\n", i, cmd->args[i]);
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
