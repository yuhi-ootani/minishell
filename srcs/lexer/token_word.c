/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:34:11 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/24 17:21:17 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// static bool	is_unquoted_space(const char *input, size_t *i, char quote);

// static void	double_word_capacity(char **word, size_t *capacity)
// {
// 	size_t	old_capacity;
// 	char	*new_word;

// 	old_capacity = *capacity;
// 	*capacity *= 2;
// 	new_word = ft_realloc(*word, old_capacity, *capacity);
// 	if (!new_word)
// 		exit(EXIT_FAILURE); // TODO: add better error handling
// 	*word = new_word;
// }

// static bool	is_quote(const char *input, size_t *i, char *quote)
// {
// 	if (input[*i] == '\'' || input[*i] == '"')
// 	{
// 		if (!(*quote))
// 		{
// 			*quote = input[*i];
// 			(*i)++;
// 			return (true);
// 		}
// 		else if (input[*i] == *quote)
// 		{
// 			*quote = 0;
// 			(*i)++;
// 			return (true);
// 		}
// 	}
// 	return (false);
// }

// static bool	is_unquoted_space(const char *input, size_t *i, char quote)
// {
// 	return (!quote && ft_strchr(delimiters, input[*i]));
// }

// static char	*extract_word(const char *input, size_t *i)
// {
// 	char	*word;
// 	size_t	capacity;
// 	size_t	len;
// 	char	quote;

// 	len = 0;
// 	quote = 0;
// 	capacity = 64;
// 	word = (char *)malloc(capacity);
// 	if (!word)
// 		exit(EXIT_FAILURE); // must modified !!!
// 	while (input[*i] && !is_unquoted_space(input, i, quote))
// 	{
// 		if (is_quote(input, i, &quote))
// 		{
// 			if (len + 1 >= capacity)
// 				double_word_capacity(&word, &capacity);
// 			word[len++] = input[(*i) - 1];
// 			continue ;
// 		}
// 		if (!quote && ft_strchr("|><", input[*i]))
// 			break ;
// 		if (len + 1 >= capacity)
// 			double_word_capacity(&word, &capacity);
// 		word[len++] = input[(*i)++];
// 	}
// 	word[len] = '\0';
// 	return (word);
// }

// static bool	is_unquoted_space(const char *input, size_t *i, char quote);

// static void	double_word_capacity(char **word, size_t *capacity)
// {
// 	size_t	old_capacity;
// 	char	*new_word;

// 	old_capacity = *capacity;
// 	*capacity *= 2;
// 	new_word = ft_realloc(*word, old_capacity, *capacity);
// 	if (!new_word)
// 		exit(EXIT_FAILURE); // TODO: add better error handling
// 	*word = new_word;
// }

static char	is_in_quote(char current_c, char *quote)
{
	if (current_c == '\'' || current_c == '"')
	{
		if (!(*quote))
		{
			return (current_c);
		}
		else if (current_c == *quote)
		{
			return (0);
		}
	}
	return (*quote);
}

static bool	is_unquoted_delimiter_or_operator(char current_c, char quote)
{
	if (!quote && ft_strchr(DELIMITERS, current_c))
		return (true);
	else if (!quote && ft_strchr("|><", current_c))
		return (true);
	else
		return (false);
}

size_t	calculate_word_length(const char *input, size_t *i)
{
	size_t	len;
	char	quote;

	quote = 0;
	len = 0;
	while (input[*i] && !is_unquoted_delimiter_or_operator(input[*i], quote))
	{
		quote = is_in_quote(input[*i], &quote);
		len++;
		(*i)++;
	}
	return (len);
}

static char	*extract_word(t_minishell *shell, size_t *i)
{
	char	*word;
	size_t	len;
	size_t	start;
	char	*input;

	start = *i;
	input = shell->input;
	len = calculate_word_length(input, i);
	word = (char *)ft_calloc(len + 1, sizeof(char));
	if (!word)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	ft_memcpy(word, input + start, len);
	word[len] = '\0';
	return (word);
}

bool	add_word_token(t_minishell *shell, size_t *i, t_token **tokens)
{
	char	*word;
	bool	malloc_fail;

	malloc_fail = false;
	word = extract_word(shell, i);
	if (!word)
		return (EXIT_FAILURE);
	if (*word != '\0')
	{
		malloc_fail = add_token(tokens, create_token(shell, TOKEN_WORD, word));
		free(word);
	}
	else if (*word == '\0')
		free(word);
	return (malloc_fail);
}
