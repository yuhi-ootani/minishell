/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:09:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/17 19:59:14 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(t_env *env, const char *name)
{
	if (!env || !name)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	append_to_result(t_expanded_str *status, const char *src,
		size_t src_len)
{
	char	*tmp;

	if (status->index + src_len >= status->size)
	{
		status->size = status->index + src_len + 1;
		tmp = realloc(status->buffer, status->size);
		if (!tmp)
			return (-1);
		status->buffer = tmp;
	}
	memcpy(status->buffer + status->index, src, src_len);
	status->index += src_len;
	status->buffer[status->index] = '\0';
	return (0);
}

static int	handle_exit_status(t_expanded_str *status)
{
	char	buf[12];

	snprintf(buf, sizeof(buf), "%d", 0); // snprintf
	return (append_to_result(status, buf, ft_strlen(buf)));
}
static int	expand_variable(const char *input, size_t *i,
		t_expanded_str *status, t_env *copied_env)
{
	size_t	var_len;
	char	*name;
	char	*val;

	var_len = 0;
	while (isalnum(input[*i + var_len]) || input[*i + var_len] == '_')
		var_len++;
	if (var_len > 0)
	{
		name = malloc(var_len + 1);
		if (!name)
			return (-1);
		strncpy(name, input + *i, var_len);
		name[var_len] = '\0';
		val = get_env_value(copied_env, name);
		free(name);
		if (val)
		{
			if (append_to_result(status, val, strlen(val)) == -1)
				return (-1);
		}
		*i += var_len;
	}
	else
	{
		if (append_to_result(status, "$", 1) == -1)
			return (-1);
	}
	return (0);
}

static int	decide_if_expand_or_not(const char *input, t_expanded_str *status,
		t_env *copied_env)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !status->in_double_quote)
		{
			status->in_single_quote = !status->in_single_quote;
			if (append_to_result(status, &input[i], 1) == -1)
				return (-1);
			i++;
			continue ;
		}
		if (input[i] == '"' && !status->in_single_quote)
		{
			status->in_double_quote = !status->in_double_quote;
			if (append_to_result(status, &input[i], 1) == -1)
				return (-1);
			i++;
			continue ;
		}
		if (input[i] == '$' && !status->in_single_quote)
		{
			i++;
			if (input[i] == '?') // this will move to executor
			{
				if (handle_exit_status(status) == -1)
					return (-1);
				i++;
			}
			else
			{
				if (expand_variable(input, &i, status, copied_env) == -1)
					return (-1);
			}
			continue ;
		}
		if (append_to_result(status, &input[i], 1) == -1)
			return (-1);
		i++;
	}
	return (0);
}

char	*set_argument_for_expansion(const char *input, t_env *copied_env)
{
	t_expanded_str	expanded_str;

	expanded_str.size = strlen(input) + 1;
	expanded_str.index = 0;
	expanded_str.in_single_quote = false;
	expanded_str.in_double_quote = false;
	expanded_str.buffer = malloc(expanded_str.size);
	if (!expanded_str.buffer)
		return (NULL);
	expanded_str.buffer[0] = '\0';
	if (decide_if_expand_or_not(input, &expanded_str, copied_env))
	{
		free(expanded_str.buffer);
		return (NULL);
	}
	return (expanded_str.buffer);
}

char	*remove_quotes(const char *input)
{
	char	*result;
	size_t	j;
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	result = malloc(strlen(input) + 1);
	if (!result)
		return (NULL);
	j = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double)
			in_single = !in_single;
		else if (*input == '"' && !in_single)
			in_double = !in_double;
		else
			result[j++] = *input;
		input++;
	}
	result[j] = '\0';
	return (result);
}

void	expand_commands(t_minishell *shell)
{
	t_command	*current;
	char		**new_args;

	current = shell->commands;
	while (current)
	{
		new_args = expander(shell, current->args);
		ft_array_free(current->args);
		current->args = new_args;
		current = current->next;
	}
}
