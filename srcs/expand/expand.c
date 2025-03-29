/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:09:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/29 14:19:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	*get_env_name(t_minishell *shell, const char *input)
{
	size_t	len;
	char	*name;

	if (!input)
		return (NULL);
	if (ft_isalpha(input[0]) || input[0] == '_')
		len = 1;
	else
		return (NULL);
	while (ft_isalnum(input[len]) || input[len] == '_')
		len++;
	name = (char *)ft_calloc((len + 1), sizeof(char));
	if (!name)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	if (len > 0)
		ft_strncpy(name, input, len);
	name[len] = '\0';
	return (name);
}

static bool	append_to_buffer(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src, size_t src_len)
{
	char	*new_buffer;
	size_t	old_size;
	size_t	new_size;

	if (expanded_str->index + src_len >= expanded_str->size)
	{
		old_size = expanded_str->size;
		new_size = expanded_str->index + src_len + 1;
		new_buffer = ft_realloc(expanded_str->buffer, old_size, new_size);
		if (!new_buffer)
		{
			shell->exit_status = EXIT_FAILURE;
			return (false);
		}
		expanded_str->buffer = new_buffer;
		expanded_str->size = new_size;
	}
	ft_memcpy(expanded_str->buffer + expanded_str->index, src, src_len);
	expanded_str->index += src_len;
	expanded_str->buffer[expanded_str->index] = '\0';
	return (true);
}

bool	append_one_char(t_minishell *shell, t_expanded_str *s_expanded_str,
		const char *src_input, size_t *i)
{
	if (src_input[*i] == '\'' && !s_expanded_str->in_double_quote)
	{
		s_expanded_str->in_single_quote = !s_expanded_str->in_single_quote;
	}
	else if (src_input[*i] == '"' && !s_expanded_str->in_single_quote)
	{
		s_expanded_str->in_double_quote = !s_expanded_str->in_double_quote;
	}
	if (!append_to_buffer(shell, s_expanded_str, &src_input[*i], 1))
		return (false);
	(*i)++;
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
	value = get_env_value(shell, name);
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

static bool	append_exit_status(t_minishell *shell, t_expanded_str *expanded_str)
{
	char	*exit_status;

	exit_status = ft_itoa(shell->exit_status);
	if (!exit_status)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	if (!append_to_buffer(shell, expanded_str, exit_status,
			ft_strlen(exit_status)))
	{
		free(exit_status);
		return (false);
	}
	free(exit_status);
	return (true);
}

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

bool	is_quote_or_delimiter_char(char c)
{
	return (c == '\'' || c == '"' || ft_strchr(DELIMITERS, c));
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

bool	init_expanded_str(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input)
{
	expanded_str->size = ft_strlen(src_input) + 1;
	expanded_str->index = 0;
	expanded_str->in_single_quote = false;
	expanded_str->in_double_quote = false;
	expanded_str->buffer = ft_calloc(sizeof(char), expanded_str->size);
	if (!expanded_str->buffer)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	return (true);
}

char	*get_expanded_str(t_minishell *shell, const char *src_input)
{
	t_expanded_str	expanded_str;
	size_t			i;

	if (!src_input)
		return (NULL);
	if (!init_expanded_str(shell, &expanded_str, src_input))
		return (NULL);
	i = 0;
	while (src_input[i])
	{
		if (src_input[i] == '$' && !expanded_str.in_single_quote)
		{
			i++;
			if (!handle_dollar(shell, &expanded_str, src_input, &i))
				return (free(expanded_str.buffer), NULL);
		}
		else
		{
			if (!append_one_char(shell, &expanded_str, src_input, &i))
				return (free(expanded_str.buffer), NULL);
		}
	}
	return (expanded_str.buffer);
}

// static int	append_to_buffer(t_expanded_str *expanded_str, const char *src,
// 		size_t src_len)
// {
// 	char	*new_buffer;

// 	if (expanded_str->index + src_len >= expanded_str->size)
// 	{
// 		expanded_str->size = expanded_str->index + src_len + 1;
// 		new_buffer = realloc(expanded_str->buffer, expanded_str->size);
// 		if (!new_buffer)
// 			return (-1);
// 		expanded_str->buffer = new_buffer;
// 	}
// 	ft_memcpy(expanded_str->buffer + expanded_str->index, src, src_len);
// 	expanded_str->index += src_len;
// 	expanded_str->buffer[expanded_str->index] = '\0';
// 	return (0);
// }
