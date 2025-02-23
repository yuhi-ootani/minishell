/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:32:39 by oyuhi             #+#    #+#             */
/*   Updated: 2025/02/23 15:17:31 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to perform lexical analysis and tokenize the input string
t_token	*lexer(const char *input)
{
	size_t	i;
	t_token	*tokens;

	// Initialize an index to track position in input
	i = 0;
	// Head of the token list (initially empty)
	tokens = NULL;
	// Loop until the end of input
	while (input[i])
	{
		// Skip spaces and tabs
		skip_whitespace(input, &i);
		// If end of input, break loop
		if (!input[i])
			break ;
		// **Handle pipes ('|')**
		if (input[i] == '|')
			;
		// Create and add pipe token
		// Move to the next character
		// **Handle input redirection ('<' and '<<')**
		else if (input[i] == '<')
			;
		// Check for heredoc (`<<`)
		// Skip both '<' characters
		// Single '<' (input redirection)
		// **Handle output redirection ('>' and '>>')**
		else if (input[i] == '>')
			;
		// Check for append (`>>`)
		// Skip both '>' characters
		// Single '>' (output redirection)
		// **Handle environment variables ('$VAR' and special case '$?')**
		else if (input[i] == '$')
			;
		// Skip the '$' character
		// Special case: `$?` (last exit status)
		// Move past '?'
		// Normal environment variable ($VAR_NAME)
		// Record starting position
		// Continue until non-alphanumeric character
		// Calculate length of variable name
		// Allocate memory for the variable name with '$' prefix
		// First character is always '$'
		// Copy variable name
		// Null-terminate the string
		// Add token to list
		// Free temporary memory
		// **Handle generic arguments (commands, filenames, etc.)**
		else
			;
		// Extract a full argument
		// Ensure it's not empty
		// Add argument token
		// Free temporary memory after adding token
	}
	// **Append an end-of-file (EOF) token at the end**
	append_token(&tokens, new_token(TOKEN_EOF, NULL));
	// Return the linked list of tokens
	return (tokens);
}

int	main(int argc, char *argv)
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