/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/04 12:24:15 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

#include <ctype.h>

typedef struct s_builder
{
	char		*str;
	size_t		capacity;
	size_t		len;
}				t_builder;

static void	builder_init(t_builder *b)
{
	b->capacity = 64;
	b->len = 0;
	b->str = malloc(b->capacity);
	if (!b->str)
		exit(EXIT_FAILURE);
}

static void	builder_append_char(t_builder *b, char c)
{
	if (b->len + 1 >= b->capacity)
	{
		b->capacity *= 2;
		b->str = realloc(b->str, b->capacity);
		if (!b->str)
			exit(EXIT_FAILURE);
	}
	b->str[b->len++] = c;
}

static void	builder_append_str(t_builder *b, const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		builder_append_char(b, s[i]);
		i++;
	}
}

static char	*builder_finalize(t_builder *b)
{
	builder_append_char(b, '\0');
	return (b->str);
}


static void	process_dollar(const char *s, size_t *i, t_builder *b)
{
	size_t	start;
	size_t	j;
	size_t	var_len;
	char	*var_name;
	char	*env_value;

	start = *i + 1;
	j = start;
	while (s[j] && (isalnum(s[j]) || s[j] == '_'))
		j++;
	if (j == start)
	{
		builder_append_char(b, s[*i]);
		(*i)++;
		return ;
	}
	var_len = j - start;
	var_name = malloc(var_len + 1);
	if (!var_name)
		exit(EXIT_FAILURE);
	memcpy(var_name, s + start, var_len);
	var_name[var_len] = '\0';
	env_value = getenv(var_name);
	free(var_name);
	if (env_value)
		builder_append_str(b, env_value);
	*i = j;
}

static char	*expand_env_variables(const char *s)
{
	t_builder	b;
	size_t		i;

	i = 0;
	builder_init(&b);
	while (s[i])
	{
		if (s[i] == '$')
			process_dollar(s, &i, &b);
		else
		{
			builder_append_char(&b, s[i]);
			i++;
		}
	}
	return (builder_finalize(&b));
}

/* --- POMOCNÉ FUNKCE PRO ZPRACOVÁNÍ SEGMENTŮ ARGUMENTU --- */
static size_t	process_single_quote(const char *arg, size_t i)
{
	i++;
	while (arg[i] && arg[i] != '\'')
	{
		write(1, &arg[i], 1);
		i++;
	}
	if (arg[i] == '\'')
		i++;
	return (i);
}

static size_t	process_double_quote(const char *arg, size_t i)
{
	size_t	start;
	char	*segment;
	char	*expanded;

	i++;
	start = i;
	while (arg[i] && arg[i] != '"')
		i++;
	segment = ft_substr(arg, start, i - start);
	expanded = expand_env_variables(segment);
	ft_putstr_fd(expanded, 1);
	free(segment);
	free(expanded);
	if (arg[i] == '"')
		i++;
	return (i);
}

static size_t	process_unquoted_segment(const char *arg, size_t i)
{
	size_t	start;
	char	*segment;
	char	*expanded;

	start = i;
	while (arg[i] && arg[i] != '\'' && arg[i] != '"')
		i++;
	segment = ft_substr(arg, start, i - start);
	expanded = expand_env_variables(segment);
	ft_putstr_fd(expanded, 1);
	free(segment);
	free(expanded);
	return (i);
}

static void	process_argument(char *arg)
{
	size_t	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '\'')
			i = process_single_quote(arg, i);
		else if (arg[i] == '"')
			i = process_double_quote(arg, i);
		else
			i = process_unquoted_segment(arg, i);
	}
}

/* --- OSTATNÍ FUNKCE --- */
static int	is_option_n(char *arg)
{
	int	j;

	if (!arg || arg[0] != '-')
		return (0);
	j = 1;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (0);
		j++;
	}
	return (1);
}

int	ft_echo(t_command *command)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	while (command->args[i] && is_option_n(command->args[i]))
	{
		n_option = 1;
		i++;
	}
	while (command->args[i])
	{
		process_argument(command->args[i]);
		if (command->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_option)
		write(1, "\n", 1);
	return (0);
}
