/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_get_value.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:18:35 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 13:25:30 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖    ▗▖  ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄▖    ▗▖  ▗▖ ▗▄▖ ▗▖   ▗▖ ▗▖▗▄▄▄▖
// ▐▌   ▐▌     █      ▐▛▚▖▐▌▐▌ ▐▌▐▛▚▞▜▌▐▌       ▐▌  ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌▐▌
// ▐▌▝▜▌▐▛▀▀▘  █      ▐▌ ▝▜▌▐▛▀▜▌▐▌  ▐▌▐▛▀▀▘    ▐▌  ▐▌▐▛▀▜▌▐▌   ▐▌ ▐▌▐▛▀▀▘
// ▝▚▄▞▘▐▙▄▄▖  █      ▐▌  ▐▌▐▌ ▐▌▐▌  ▐▌▐▙▄▄▖     ▝▚▞▘ ▐▌ ▐▌▐▙▄▄▖▝▚▄▞▘▐▙▄▄▖

bool	is_invalid_arg(char *arg)
{
	size_t	i;

	i = 0;
	if (!arg || arg[0] == '\0')
		return (true);
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (true);
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
			break ;
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (true);
		i++;
	}
	return (false);
}

static size_t	skip_operands(const char *arg, size_t i)
{
	if (arg[i] == '+' && arg[i + 1] == '=')
	{
		return (i + 2);
	}
	else if (arg[i] == '=')
		return (i + 1);
	return (i);
}

char	*get_value(char *arg, size_t i)
{
	char	*value;

	if (arg[i])
		value = ft_strdup(arg + i);
	else
		value = ft_strdup("");
	return (value);
}

bool	get_name_and_value(char *arg, char **name, char **value,
		bool *append)
{
	size_t	i;

	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	*name = ft_strndup(arg, i);
	if (!name)
		return (false);
	*append = false;
	if (arg[i] == '+' && arg[i + 1] == '=')
		*append = true;
	i = skip_operands(arg, i);
	*value = NULL;
	if (arg[i])
	{
		*value = get_value(arg, i);
		if (!*value)
			return (free(*name), false);
	}
	return (true);
}
