/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:32:39 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/24 15:47:35 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*free_token_and_fail(t_minishell *shell, t_token *token)
{
	if (token)
		free(token);
	shell->exit_status = EXIT_FAILURE;
	return (NULL);
}

t_token	*create_token(t_minishell *shell, t_token_type type, const char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (free_token_and_fail(shell, token), NULL);
	if (value)
	{
		token->value = ft_strdup(value);
		if (!token->value)
			return (free_token_and_fail(shell, token), NULL);
	}
	else
		token->value = NULL;
	token->type = type;
	token->next = NULL;
	return (token);
}

bool	add_token(t_token **head, t_token *new_node)
{
	t_token	*temp;

	if (!new_node)
		return (EXIT_FAILURE);
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
	return (EXIT_SUCCESS);
}

bool	add_redirection_token(t_minishell *shell, size_t *i, t_token **tokens)
{
	char	*input;

	input = shell->input;
	if (input[*i] == '<')
	{
		(*i)++;
		if (input[*i] == '<')
		{
			(*i)++;
			return (add_token(tokens, create_token(shell, TOKEN_HEREDOC,
						"<<")));
		}
		return (add_token(tokens, create_token(shell, TOKEN_REDIR_IN, "<")));
	}
	else if (input[*i] == '>')
	{
		(*i)++;
		if (input[*i] == '>')
		{
			(*i)++;
			return (add_token(tokens, create_token(shell, TOKEN_APPEND, ">>")));
		}
		return (add_token(tokens, create_token(shell, TOKEN_REDIR_OUT, ">")));
	}
	return (EXIT_FAILURE);
}

bool	add_pipe_token(t_minishell *shell, size_t *i, t_token **tokens)
{
	(*i)++;
	return (add_token(tokens, create_token(shell, TOKEN_PIPE, "|")));
}

bool	add_EOF_token(t_minishell *shell, t_token **tokens)
{
	return (add_token(tokens, create_token(shell, TOKEN_EOF, NULL)));
}

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && isspace(input[*i]))
		(*i)++;
}

t_token	*tokenizer(t_minishell *shell)
{
	size_t	i;
	t_token	*tokens;
	char	*input;
	bool	malloc_fail;

	malloc_fail = false;
	input = shell->input;
	i = 0;
	tokens = NULL;
	while (input[i] && !malloc_fail)
	{
		skip_whitespace(input, &i);
		if (input[i] && input[i] == '|')
			malloc_fail = add_pipe_token(shell, &i, &tokens);
		else if (input[i] && (input[i] == '<' || input[i] == '>'))
			malloc_fail = add_redirection_token(shell, &i, &tokens);
		else if (input[i])
			malloc_fail = add_word_token(shell, &i, &tokens);
	}
	if (!malloc_fail)
		malloc_fail = add_EOF_token(shell, &tokens);
	if (malloc_fail)
		return (free_tokens(tokens), NULL);
	return (tokens);
}
