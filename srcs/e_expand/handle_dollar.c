/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:37:29 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 12:02:01 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_printable_dollar(t_expanded_str *expanded_str, const char *src_input,
		size_t i)
{
	if (src_input[i] == '\0')
		return (true);
	else if (expanded_str->in_single_quote)
		return (true);
	else if (!expanded_str->in_double_quote && src_input[i] != '\0')
		return (false);
	else if (expanded_str->in_double_quote && (ft_isalnum(src_input[i])
			|| src_input[i] == '_' || src_input[i] == '?'))
		return (false);
	else
		return (true);
}

bool	append_env_value(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input, size_t *i)
{
	size_t	name_len;
	char	*name;
	char	*value;

	name = get_env_name(shell, src_input + *i);
	if (!name)
		return (false);
	name_len = ft_strlen(name);
	*i += name_len;
	if (!get_env_value(shell, name, &value))
	{
		free(name);
		return (false);
	}
	free(name);
	if (value)
	{
		if (!append_to_buffer(shell, expanded_str, value, ft_strlen(value)))
		{
			free(value);
			return (false);
		}
		free(value);
	}
	return (true);
}

bool	handle_dollar(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input, size_t *i)
{
	if (is_printable_dollar(expanded_str, src_input, *i))
		return (append_to_buffer(shell, expanded_str, "$", 1));
	else if (src_input[*i] == '?')
	{
		(*i)++;
		return (append_exit_status(shell, expanded_str));
	}
	else if (ft_isalpha(src_input[*i]) || src_input[*i] == '_')
	{
		return (append_env_value(shell, expanded_str, src_input, i));
	}
	else if (is_quote_or_delimiter_char(src_input[*i]))
		return (true);
	(*i)++;
	return (true);
}
