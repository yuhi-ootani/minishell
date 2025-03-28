/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:09:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/27 10:20:01 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(t_env *env, const char *name)
{
	char	*copied_value;

	if (!env || !name)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			copied_value = ft_strdup(env->value);
			return (copied_value);
		}
		env = env->next;
	}
	return (NULL);
}

char	*get_env_name(const char *input)
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
		return (NULL);
	if (len > 0)
		ft_strncpy(name, input, len);
	name[len] = '\0';
	return (name);
}

// static int	append_to_buffer(t_expanded_str *expanded_str, const char *src,
// 		size_t src_len)
// {
// 	char	*new_buffer;

// 	if (expanded_str->index + src_len >= expanded_str->size)
// 	{
// 		expanded_str->size = expanded_str->index + src_len + 1;
// 		new_buffer = ft_realloc(expanded_str->buffer, expanded_str->size);
// 		if (!new_buffer)
// 			return (-1);
// 		expanded_str->buffer = new_buffer;
// 	}
// 	ft_memcpy(expanded_str->buffer + expanded_str->index, src, src_len);
// 	expanded_str->index += src_len;
// 	expanded_str->buffer[expanded_str->index] = '\0';
// 	return (0);
// }

static int	append_to_buffer(t_expanded_str *expanded_str, const char *src,
		size_t src_len)
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
			return (-1);
		expanded_str->buffer = new_buffer;
		expanded_str->size = new_size;
	}
	ft_memcpy(expanded_str->buffer + expanded_str->index, src, src_len);
	expanded_str->index += src_len;
	expanded_str->buffer[expanded_str->index] = '\0';
	return (0);
}

static int	append_exit_status(t_minishell *shell, t_expanded_str *expanded_str)
{
	char	*exit_status;

	exit_status = ft_itoa(shell->exit_status);
	if (!exit_status)
		return (-1);
	if (append_to_buffer(expanded_str, exit_status, ft_strlen(exit_status)) ==
		-1)
	{
		free(exit_status);
		return (-1);
	}
	free(exit_status);
	return (0);
}

int	append_env_value(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input, size_t *i)
{
	size_t	name_len;
	char	*name;
	char	*value;

	name = get_env_name(src_input + *i);
	if (!name)
		return (-1);
	name_len = ft_strlen(name);
	*i += name_len;
	value = get_env_value(shell->env, name);
	free(name);
	if (value)
	{
		if (append_to_buffer(expanded_str, value, ft_strlen(value)) == -1)
		{
			free(value);
			return (-1);
		}
		free(value);
	}
	return (0);
}

int	append_one_char(t_expanded_str *s_expanded_str, const char *src_input,
		size_t *i)
{
	if (src_input[*i] == '\'' && !s_expanded_str->in_double_quote)
	{
		s_expanded_str->in_single_quote = !s_expanded_str->in_single_quote;
	}
	else if (src_input[*i] == '"' && !s_expanded_str->in_single_quote)
	{
		s_expanded_str->in_double_quote = !s_expanded_str->in_double_quote;
	}
	if (append_to_buffer(s_expanded_str, &src_input[*i], 1) == -1)
		return (-1);
	(*i)++;
	return (0);
}

int	handle_dollar(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input, size_t *i)
{
	if (src_input[*i] == '\0' || (src_input[*i] == '"' && src_input[*i
			+ 1] == '\0'))
		return (append_to_buffer(expanded_str, "$", 1));
	else if (src_input[*i] == '?')
	{
		(*i)++;
		return (append_exit_status(shell, expanded_str));
	}
	else if (ft_isalpha(src_input[*i]) || src_input[*i] == '_')
	{
		return (append_env_value(shell, expanded_str, src_input, i));
	}
	else if (src_input[*i] == '\'' || src_input[*i] == '"')
		return (0);
	(*i)++;
	return (0);
}

static t_expanded_str	init_expanded_str(t_minishell *shell,
		const char *arg_src)
{
	t_expanded_str	expanded_str;

	expanded_str.size = ft_strlen(arg_src) + 1;
	expanded_str.index = 0;
	expanded_str.in_single_quote = false;
	expanded_str.in_double_quote = false;
	expanded_str.buffer = ft_calloc(sizeof(char), expanded_str.size);
	if (!expanded_str.buffer)
	{
		shell->exit_status = EXIT_FAILURE;
	}
	return (expanded_str);
}

char	*get_expanded_str(t_minishell *shell, const char *src_input)
{
	t_expanded_str	expanded_str;
	size_t			i;

	if (!src_input)
		return (NULL);
	expanded_str = init_expanded_str(shell, src_input);
	if (!expanded_str.buffer)
		return (NULL);
	i = 0;
	while (src_input[i])
	{
		if (src_input[i] == '$' && !expanded_str.in_single_quote)
		{
			i++;
			if (handle_dollar(shell, &expanded_str, src_input, &i) == -1)
				return (free(expanded_str.buffer), NULL);
		}
		else
		{
			if (append_one_char(&expanded_str, src_input, &i) == -1)
				return (free(expanded_str.buffer), NULL);
		}
	}
	return (expanded_str.buffer);
}
