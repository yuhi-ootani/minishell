/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:09:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/14 14:48:00 by knemcova         ###   ########.fr       */
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

static int	append_to_result(t_expstate *st, const char *src, size_t src_len)
{
	char	*tmp;

	if (st->result_index + src_len >= st->result_size)
	{
		st->result_size = st->result_index + src_len + 1;
		tmp = realloc(st->result, st->result_size);
		if (!tmp)
			return (-1);
		st->result = tmp;
	}
	memcpy(st->result + st->result_index, src, src_len);
	st->result_index += src_len;
	st->result[st->result_index] = '\0';
	return (0);
}

static int	handle_exit_status(t_expstate *st)
{
	char	buf[12];

	snprintf(buf, sizeof(buf), "%d", 0); // snprintf
	return (append_to_result(st, buf, ft_strlen(buf)));
}

static int	expand_variable(const char *input, size_t *i, t_expstate *st, t_env *copied_env)
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
			if (append_to_result(st, val, strlen(val)) == -1)
				return (-1);
		}
		*i += var_len;
	}
	else
	{
		if (append_to_result(st, "$", 1) == -1)
			return (-1);
	}
	return (0);
}

static int	decide_if_expand_or_not(const char *input, t_expstate *st, t_env *copied_env)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !st->in_double)
		{
			st->in_single = !st->in_single;
			if (append_to_result(st, &input[i], 1) == -1)
				return (-1);
			i++;
			continue ;
		}
		if (input[i] == '"' && !st->in_single)
		{
			st->in_double = !st->in_double;
			if (append_to_result(st, &input[i], 1) == -1)
				return (-1);
			i++;
			continue ;
		}
		if (input[i] == '$' && !st->in_single)
		{
			i++;
			if (input[i] == '?') // this will move to executor
			{
				if (handle_exit_status(st) == -1)
					return (-1);
				i++;
			}
			else
			{
				if (expand_variable(input, &i, st, copied_env) == -1)
					return (-1);
			}
			continue ;
		}
		if (append_to_result(st, &input[i], 1) == -1)
			return (-1);
		i++;
	}
	return (0);
}

char	*set_argument_for_expansion(const char *input, t_env *copied_env)
{
	t_expstate	exp_state;

	exp_state.result_size = strlen(input) + 1;
	exp_state.result_index = 0;
	exp_state.in_single = false;
	exp_state.in_double = false;
	exp_state.result = malloc(exp_state.result_size);
	if (!exp_state.result)
		return (NULL);
	exp_state.result[0] = '\0';
	if (decide_if_expand_or_not(input, &exp_state, copied_env))
	{
		free(exp_state.result);
		return (NULL);
	}
	return (exp_state.result);
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

char	*process_argument(const char *input, t_env *copied_env)
{
	char	*expanded;
	char	*no_quotes;

	expanded = set_argument_for_expansion(input, copied_env);
	if (!expanded)
		return (NULL);
	no_quotes = remove_quotes(expanded);
	free(expanded);
	return (no_quotes);
}

void	expand_commands(t_command *command_list, t_env *copied_env)
{
	t_command	*current;
	size_t		i;
	char		*arg;

	current = command_list;
	while (current)
	{
		i = 0;
		while (current->args[i])
		{
			arg = process_argument(current->args[i], copied_env);
			if (arg)
			{
				free(current->args[i]);
				current->args[i] = arg;
			}
			else
			{
				fprintf(stderr, "Error expanding argument: %s\n",
					current->args[i]);
			}
			i++;
		}
		current = current->next;
	}
}

// static int	append_to_result(t_expstate *st, const char *src,
// size_t src_len)
// {
// 	char	*tmp;

// 	if (st->result_index + src_len >= st->result_size)
// 	{
// 		st->result_size = st->result_index + src_len + 1;
// 		tmp = realloc(st->result, st->result_size);
// 		if (!tmp)
// 			return (-1);
// 		st->result = tmp;
// 	}
// 	memcpy(st->result + st->result_index, src, src_len);
// 	st->result_index += src_len;
// 	st->result[st->result_index] = '\0';
// 	return (0);
// }

// static int	handle_exit_status(t_expstate *st)
// {
// 	char	buf[12];

// 	snprintf(buf, sizeof(buf), "%d", 0); // snprintf
// 	return (append_to_result(st, buf, ft_strlen(buf)));
// }

// static int	expand_variable(const char *input, size_t *i, t_expstate *st)
// {
// 	size_t	var_len;
// 	char	*name;
// 	char	*val;

// 	var_len = 0;
// 	while (isalnum(input[*i + var_len]) || input[*i + var_len] == '_')
// 		var_len++;
// 	if (var_len > 0)
// 	{
// 		name = malloc(var_len + 1);
// 		if (!name)
// 			return (-1);
// 		strncpy(name, input + *i, var_len);
// 		name[var_len] = '\0';
// 		val = getenv(name);
// 		free(name);
// 		if (val && append_to_result(st, val, strlen(val)))
// 			return (-1);
// 		*i += var_len;
// 	}
// 	else
// 	{
// 		if (append_to_result(st, "$", 1))
// 			return (-1);
// 	}
// 	return (0);
// }

// static int	decide_if_expand_or_not(const char *input, t_expstate *st)
// {
// 	size_t	i;

// 	i = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '\'' && !st->in_double)
// 		{
// 			st->in_single = !st->in_single;
// 			if (append_to_result(st, &input[i], 1))
// 				return (-1);
// 			i++;
// 			continue ;
// 		}
// 		if (input[i] == '"' && !st->in_single)
// 		{
// 			st->in_double = !st->in_double;
// 			if (append_to_result(st, &input[i], 1))
// 				return (-1);
// 			i++;
// 			continue ;
// 		}
// 		if (input[i] == '$' && !st->in_single)
// 		{
// 			i++;
// 			if (input[i] == '?') // this will move to executor
// 			{
// 				if (handle_exit_status(st))
// 					return (-1);
// 				i++;
// 			}
// 			else
// 			{
// 				if (expand_variable(input, &i, st))
// 					return (-1);
// 			}
// 			continue ;
// 		}
// 		if (append_to_result(st, &input[i], 1))
// 			return (-1);
// 		i++;
// 	}
// 	return (0);
// }

// char	*set_argument_for_expansion(const char *input)
// {
// 	t_expstate	exp_state;

// 	exp_state.result_size = strlen(input) + 1;
// 	exp_state.result_index = 0;
// 	exp_state.in_single = false;
// 	exp_state.in_double = false;
// 	exp_state.result = malloc(exp_state.result_size);
// 	if (!exp_state.result)
// 		return (NULL);
// 	exp_state.result[0] = '\0';
// 	if (decide_if_expand_or_not(input, &exp_state))
// 	{
// 		free(exp_state.result);
// 		return (NULL);
// 	}
// 	return (exp_state.result);
// }

// char	*remove_quotes(const char *input)
// {
// 	char	*result;
// 	size_t	j;
// 	bool	in_single;
// 	bool	in_double;

// 	in_single = false;
// 	in_double = false;
// 	result = malloc(strlen(input) + 1);
// 	if (!result)
// 		return (NULL);
// 	j = 0;
// 	while (*input)
// 	{
// 		if (*input == '\'' && !in_double)
// 			in_single = !in_single;
// 		else if (*input == '"' && !in_single)
// 			in_double = !in_double;
// 		else
// 			result[j++] = *input;
// 		input++;
// 	}
// 	result[j] = '\0';
// 	return (result);
// }

// char	*process_argument(const char *input)
// {
// 	char	*expanded;
// 	char	*no_quotes;

// 	expanded = set_argument_for_expansion(input);
// 	if (!expanded)
// 		return (NULL);
// 	no_quotes = remove_quotes(expanded);
// 	free(expanded);
// 	return (no_quotes);
// }

// void	expand_commands(t_command *command_list)
// {
// 	t_command	*current;
// 	size_t		i;
// 	char		*arg;

// 	current = command_list;
// 	while (current)
// 	{
// 		i = 0;
// 		while (current->args[i])
// 		{
// 			arg = process_argument(current->args[i]);
// 			if (arg)
// 			{
// 				free(current->args[i]);
// 				current->args[i] = arg;
// 			}
// 			else
// 			{
// 				fprintf(stderr, "Error expanding argument: %s\n",
// 					current->args[i]);
// 			}
// 			i++;
// 		}
// 		current = current->next;
// 	}
// }
