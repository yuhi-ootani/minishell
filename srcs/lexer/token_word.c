/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:34:11 by oyuhi             #+#    #+#             */
/*   Updated: 2025/02/24 17:14:20 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_quote(const char *input, size_t *i, char *quote)
{
	if (input[*i] == '\'' || input[*i] == '"')
	{
		if (!(*quote))
		{
			*quote = input[*i];
			(*i)++;
			return (true);
		}
		else if (input[*i] == *quote)
		{
			*quote = 0;
			(*i)++;
			return (true);
		}
	}
	return (false);
}

static void	double_word_capacity(char *word, size_t *capacity)
{
	(*capacity) *= 2;
	word = (char *)realloc(word, *capacity);
	if (!word)
		exit(EXIT_FAILURE); // exit status!!!
}

static bool	is_unquoted_space(const char *input, size_t *i, char *quote)
{
	return (!(*quote) && isspace(input[*i]));
}

static char	*extract_word(const char *input, size_t *i)
{
	char	*word;
	size_t	capacity;
	size_t	len;
	char	quote;

	len = 0;
	quote = 0;
	capacity = 64;
	word = (char *)malloc(capacity);
	if (!word)
		exit(EXIT_FAILURE); // must modified !!!
	while (input[*i] && !is_unquoted_space(input, i, &quote))
	{
		if (is_quote(input, i, &quote) == true)
			continue ;
		if (!quote && strchr("|><$", input[*i])) // strchr
			break ;
		if (len + 1 >= capacity)
			double_word_capacity(word, &capacity);
		word[len++] = input[(*i)++];
	}
	word[len] = '\0';
	return (word);
}

void	append_word_token(const char *input, size_t *i, t_token **tokens)
{
	char	*word;

	word = extract_word(input, i);
	if (word && *word != '\0')
	{
		append_token(tokens, create_new_token(TOKEN_WORD, word));
		free(word);
	}
	else if (*word == '\0')
		free(word);
}
