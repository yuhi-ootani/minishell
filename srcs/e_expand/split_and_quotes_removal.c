/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_and_quotes_removal.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:16:01 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/10 09:40:54 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	quote_removal_args(t_minishell *shell, char **args)
{
	char	*tmp;
	size_t	i;

	i = 0;
	while (args && args[i])
	{
		tmp = remove_quotes_util(args[i]);
		if (tmp)
		{
			free(args[i]);
			args[i] = tmp;
		}
		else
		{
			return (false);
			shell->exit_status = EXIT_FAILURE;
		}
		i++;
	}
	return (true);
}

char	**word_splitting(t_minishell *shell, char *str)
{
	char	**splited_args;

	splited_args = split_quoted_words_util(str, DELIMITERS);
	if (!splited_args)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	return (splited_args);
}
