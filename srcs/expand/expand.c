/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:09:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/11 12:00:38 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// //correct function its just too big
// char	*expand_variables(const char *input)
// {
// 	char	*result;
// 	size_t	var_len;
// 	char	*var_name;
// 	char	*var_value;
// 	size_t	j;
// 	bool	in_single_quotes;
// 	bool	in_double_quotes;
// 	char	exit_status[12];
// 	size_t	result_size;
// 	size_t	exit_status_len;
// 	size_t	var_value_len;

// 	in_single_quotes = false;
// 	in_double_quotes = false;
// 	result_size = strlen(input) + 1;
// 	result = malloc(result_size);
// 	if (!result)
// 		return (NULL);
// 	j = 0;
// 	for (size_t i = 0; input[i]; i++)
// 	{
// 		if (input[i] == '\'' && !in_double_quotes)
// 		{
// 			in_single_quotes = !in_single_quotes;
// 			result[j++] = input[i];
// 			continue ;
// 		}
// 		if (input[i] == '"' && !in_single_quotes)
// 		{
// 			in_double_quotes = !in_double_quotes;
// 			result[j++] = input[i];
// 			continue ;
// 		}
// 		if (input[i] == '$' && !in_single_quotes)
// 		{
// 			i++;
// 			if (input[i] == '?')
// 			{
// 				snprintf(exit_status, sizeof(exit_status), "%d", 0);
// 				exit_status_len = strlen(exit_status);
// 				if (j + exit_status_len >= result_size)
// 				{
// 					result_size = j + exit_status_len + 1;
// 					result = realloc(result, result_size);
// 					if (!result)
// 						return (NULL);
// 				}
// 				memcpy(result + j, exit_status, exit_status_len);
// 				j += exit_status_len;
// 			}
// 			else
// 			{
// 				var_len = 0;
// 				while (isalnum(input[i + var_len]) || input[i + var_len] == '_')
// 					var_len++;
// 				if (var_len > 0)
// 				{
// 					var_name = malloc(var_len + 1);
// 					if (!var_name)
// 					{
// 						free(result);
// 						return (NULL);
// 					}
// 					strncpy(var_name, &input[i], var_len);
// 					var_name[var_len] = '\0';
// 					var_value = getenv(var_name);
// 					if (var_value)
// 					{
// 						var_value_len = strlen(var_value);
// 						if (j + var_value_len >= result_size)
// 						{
// 							result_size = j + var_value_len + 1;
// 							result = realloc(result, result_size);
// 							if (!result)
// 							{
// 								free(var_name);
// 								return (NULL);
// 							}
// 						}
// 						memcpy(result + j, var_value, var_value_len);
// 						j += var_value_len;
// 					}
// 					free(var_name);
// 					i += var_len - 1;
// 				}
// 				else
// 				{
// 					result[j++] = '$';
// 				}
// 			}
// 		}
// 		else
// 		{
// 			result[j++] = input[i];
// 		}
// 	}
// 	result[j] = '\0';
// 	return (result);
// }

// char	*remove_quotes(const char *input)
// {
// 	size_t	len;
// 	char	*result;
// 	size_t	j;
// 	bool	in_single_quotes;
// 	bool	in_double_quotes;

// 	len = strlen(input);
// 	result = malloc(len + 1);
// 	if (!result)
// 		return (NULL);
// 	j = 0;
// 	in_single_quotes = false;
// 	in_double_quotes = false;
// 	for (size_t i = 0; input[i]; i++)
// 	{
// 		if (input[i] == '\'' && !in_double_quotes)
// 		{
// 			in_single_quotes = !in_single_quotes;
// 			continue ;
// 		}
// 		if (input[i] == '"' && !in_single_quotes)
// 		{
// 			in_double_quotes = !in_double_quotes;
// 			continue ;
// 		}
// 		result[j++] = input[i];
// 	}
// 	result[j] = '\0';
// 	return (result);
// }

// char	*remove_quotes(const char *input)
// {
// 	char	*result;
// 	size_t	j;
// 	bool	in_single;
// 	bool	in_double;

// 	in_double = false;
// 	in_single = false;
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

// static bool	append_string(char **result, size_t *j, size_t *result_size,
// 		const char *str)
// {
// 	size_t	str_len;

// 	str_len = strlen(str);
// 	if (*j + str_len >= *result_size)
// 	{
// 		*result_size = *j + str_len + 1;
// 		*result = realloc(*result, *result_size);
// 		if (!*result)
// 			return (false);
// 	}
// 	memcpy(*result + *j, str, str_len);
// 	*j += str_len;
// 	return (true);
// }

// static bool	handle_exit_status(char **result, size_t *j,
// size_t *result_size)
// {
// 	char	exit_status[12];

// 	snprintf(exit_status, sizeof(exit_status), "%d", 0);
// 	return (append_string(result, j, result_size, exit_status));
// }

// static bool	handle_variable(const char *input, size_t *i, char **result,
// 		size_t *j, size_t *result_size)
// {
// 	size_t	var_len;
// 	char	*var_name;
// 	char	*var_value;

// 	var_len = 0;
// 	while (isalnum(input[*i + var_len]) || input[*i + var_len] == '_')
// 		var_len++;
// 	if (var_len > 0)
// 	{
// 		var_name = malloc(var_len + 1);
// 		if (!var_name)
// 			return (false);
// 		strncpy(var_name, &input[*i], var_len);
// 		var_name[var_len] = '\0';
// 		var_value = getenv(var_name);
// 		free(var_name);
// 		if (var_value && !append_string(result, j, result_size, var_value))
// 			return (false);
// 		*i += var_len - 1;
// 	}
// 	else
// 	{
// 		if (!append_string(result, j, result_size, "$"))
// 			return (false);
// 	}
// 	return (true);
// }
// static void	handle_quotes(char ch, bool *in_single_quotes,
// 		bool *in_double_quotes, char **result, size_t *j)
// {
// 	if (ch == '\'' && !*in_double_quotes)
// 		*in_single_quotes = !*in_single_quotes;
// 	else if (ch == '"' && !*in_single_quotes)
// 		*in_double_quotes = !*in_double_quotes;
// 	(*result)[(*j)++] = ch;
// }
// char	*expand_variables(const char *input)
// {
// 	char	*result;
// 	size_t	result_size;
// 	size_t	j;
// 	bool	in_single_quotes;
// 	bool	in_double_quotes;

// 	in_single_quotes = false;
// 	in_double_quotes = false;
// 	result_size = strlen(input) + 1;
// 	result = malloc(result_size);
// 	if (!result)
// 		return (NULL);
// 	j = 0;
// 	for (size_t i = 0; input[i]; i++)
// 	{
// 		if ((input[i] == '\'' && !in_double_quotes) || (input[i] == '"'
// 				&& !in_single_quotes))
// 		{
// 			handle_quotes(input[i], &in_single_quotes, &in_double_quotes,
// 				&result, &j);
// 			continue ;
// 		}
// 		if (input[i] == '$' && !in_single_quotes)
// 		{
// 			i++;
// 			if (input[i] == '?')
// 			{
// 				if (!handle_exit_status(&result, &j, &result_size))
// 				{
// 					free(result);
// 					return (NULL);
// 				}
// 			}
// 			else
// 			{
// 				if (!handle_variable(input, &i, &result, &j, &result_size))
// 				{
// 					free(result);
// 					return (NULL);
// 				}
// 			}
// 		}
// 		else
// 			result[j++] = input[i];
// 	}
// 	result[j] = '\0';
// 	return (result);
// }
// char	*expand_argument(const char *input)
// {
// 	char	*expanded;
// 	char	*no_quotes;

// 	expanded = expand_variables(input);
// 	if (!expanded)
// 		return (NULL);
// 	no_quotes = remove_quotes(expanded);
// 	free(expanded);
// 	if (!no_quotes)
// 		return (NULL);
// 	return (no_quotes);
// }

// void	expand_commands(t_command *command_list)
// {
// 	t_command	*current;
// 	size_t		i;
// 	char		*expanded_arg;

// 	current = command_list;
// 	while (current)
// 	{
// 		i = 0;
// 		while (current->args[i])
// 		{
// 			expanded_arg = expand_argument(current->args[i]);
// 			if (expanded_arg)
// 			{
// 				free(current->args[i]);
// 				current->args[i] = expanded_arg;
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

static int	append_to_result(t_expstate *st, const char *src, size_t src_len)
{
	char	*tmp;

	if (st->j + src_len >= st->result_size)
	{
		st->result_size = st->j + src_len + 1;
		tmp = realloc(st->result, st->result_size);
		if (!tmp)
			return (-1);
		st->result = tmp;
	}
	memcpy(st->result + st->j, src, src_len);
	st->j += src_len;
	st->result[st->j] = '\0';
	return (0);
}

static int	handle_exit_status(t_expstate *st)
{
	char	buf[12];

	snprintf(buf, sizeof(buf), "%d", 0); //snprintf
	return (append_to_result(st, buf, ft_strlen(buf)));
}

static int	expand_variable(const char *input, size_t *i, t_expstate *st)
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
		val = getenv(name);
		free(name);
		if (val && append_to_result(st, val, strlen(val)))
			return (-1);
		*i += var_len;
	}
	else
	{
		if (append_to_result(st, "$", 1))
			return (-1);
	}
	return (0);
}

static int	decide_if_expand_or_not(const char *input, t_expstate *st)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !st->in_double)
		{
			st->in_single = !st->in_single;
			if (append_to_result(st, &input[i], 1))
				return (-1);
			i++;
			continue ;
		}
		if (input[i] == '"' && !st->in_single)
		{
			st->in_double = !st->in_double;
			if (append_to_result(st, &input[i], 1))
				return (-1);
			i++;
			continue ;
		}
		if (input[i] == '$' && !st->in_single)
		{
			i++;
			if (input[i] == '?') //this will move to executor
			{
				if (handle_exit_status(st))
					return (-1);
				i++;
			}
			else
			{
				if (expand_variable(input, &i, st))
					return (-1);
			}
			continue ;
		}
		if (append_to_result(st, &input[i], 1))
			return (-1);
		i++;
	}
	return (0);
}

char	*set_argument_for_expansion(const char *input)
{
	t_expstate	exp_state;

	exp_state.result_size = strlen(input) + 1;
	exp_state.j = 0;
	exp_state.in_single = false;
	exp_state.in_double = false;
	exp_state.result = malloc(exp_state.result_size);
	if (!exp_state.result)
		return (NULL);
	exp_state.result[0] = '\0';
	if (decide_if_expand_or_not(input, &exp_state))
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

char	*process_argument(const char *input)
{
	char	*expanded;
	char	*no_quotes;

	expanded = set_argument_for_expansion(input);
	if (!expanded)
		return (NULL);
	no_quotes = remove_quotes(expanded);
	free(expanded);
	return (no_quotes);
}

void	expand_commands(t_command *command_list)
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
			arg = process_argument(current->args[i]);
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
