/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_and_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:16:01 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 14:25:02 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_quote_or_delimiter_char(char c)
{
	return (c == '\'' || c == '"' || ft_strchr(DELIMITERS, c));
}

void	remove_quotes_and_copy(char *dst, const char *src)
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

bool	quote_removal_args(t_minishell *shell, char **args)
{
	char	*tmp;
	size_t	i;

	i = 0;
	while (args && args[i])
	{
		tmp = remove_quotes(shell, args[i]);
		if (tmp)
		{
			free(args[i]);
			args[i] = tmp;
		}
		else
			return (false);
		i++;
	}
	return (true);
}

char	**word_splitting(t_minishell *shell, char *str)
{
	char	**splited_args;

	splited_args = ft_split_not_inquote(str, DELIMITERS);
	if (!splited_args)
	if (!splited_args)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	return (splited_args);
}
