/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:32:39 by oyuhi             #+#    #+#             */
/*   Updated: 2025/02/24 17:17:22 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*create_new_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		exit(EXIT_FAILURE);
	token->type = type;
	if (value)
		token->value = strdup(value); // strdup
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, t_token *new_node)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new_node;
	}
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

void	append_redirection_token(const char *input, size_t *i, t_token **tokens)
{
	if (input[*i] == '<')
	{
		(*i)++;
		if (input[*i] == '<')
		{
			append_token(tokens, create_new_token(TOKEN_APPEND, "<<"));
			(*i)++;
		}
		else
			append_token(tokens, create_new_token(TOKEN_REDIR_IN, "<"));
	}
	else if (input[*i] == '>')
	{
		(*i)++;
		if (input[*i] == '>')
		{
			append_token(tokens, create_new_token(TOKEN_HEREDOC, ">>"));
			(*i)++;
		}
		else
			append_token(tokens, create_new_token(TOKEN_REDIR_OUT, ">"));
	}
}

#include <ctype.h>

void	append_environment_token(const char *input, size_t *i, t_token **tokens)
{
	size_t	start;
	char	*var;
	size_t	len;

	(*i)++;
	if (input[*i] == '?')
	{
		append_token(tokens, create_new_token(TOKEN_ENV, "$?"));
		(*i)++;
	}
	else
	{
		start = *i;
		while (input[*i] && (isalnum(input[*i]) || input[*i] == '_')) // isalnum
			(*i)++;
		len = *i - start;
		var = (char *)malloc(len + 2);
		if (!var)
			exit(EXIT_FAILURE); // ! must modified
		var[0] = '$';
		strncpy(var + 1, input + start, len);
		var[len + 1] = '\0';
		append_token(tokens, create_new_token(TOKEN_ENV, var));
		free(var);
	}
}

void	append_pipe_token(size_t *i, t_token **tokens)
{
	append_token(tokens, create_new_token(TOKEN_PIPE, "|"));
	(*i)++;
}

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && isspace(input[*i]))
		(*i)++;
}

t_token	*lexer(const char *input)
{
	size_t	i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (!input[i])
			break ;
		if (input[i] == '|')
			append_pipe_token(&i, &tokens);
		else if (input[i] == '<' || input[i] == '>')
			append_redirection_token(input, &i, &tokens);
		else if (input[i] == '$')
			append_environment_token(input, &i, &tokens);
		else
			append_word_token(input, &i, &tokens);
	}
	append_token(&tokens, create_new_token(TOKEN_EOF, NULL));
	return (tokens);
}

void	print_tokens(t_token *tokens)
{
	const char	*type_str;

	while (tokens)
	{
		switch (tokens->type)
		{
		case TOKEN_WORD:
			type_str = "ARG";
			break ;
		case TOKEN_PIPE:
			type_str = "PIPE";
			break ;
		case TOKEN_REDIR_IN:
			type_str = "REDIR_IN";
			break ;
		case TOKEN_REDIR_OUT:
			type_str = "REDIR_OUT";
			break ;
		case TOKEN_APPEND:
			type_str = "REDIR_APPEND";
			break ;
		case TOKEN_HEREDOC:
			type_str = "HEREDOC";
			break ;
		case TOKEN_ENV:
			type_str = "ENV";
			break ;
		case TOKEN_EOF:
			type_str = "EOF";
			break ;
		default:
			type_str = "UNKNOWN";
			break ;
		}
		printf("[%s] '%s'\n", type_str, tokens->value ? tokens->value : "");
		tokens = tokens->next;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		t_token *tokens = lexer(argv[1]);
		printf("Input: %s\nTokens:\n", argv[1]);
		print_tokens(tokens);
		free_tokens(tokens);
	}
	return (0);
}