/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:07:23 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/17 18:53:04 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	syntax_error(const char *statement)
{
	// Print an error message to stderr and exit
	printf("%s\n", statement);
	fprintf(stderr, "Minishell: syntax error\n");
	exit(EXIT_FAILURE);
}

t_command	*create_command_node(void)
{
	t_command	*new_command;

	new_command = (t_command *)malloc(sizeof(t_command));
	if (!new_command)
		syntax_error("create_command_node"); //
	new_command->args = NULL;
	new_command->input_file = NULL;
	new_command->is_heredoc = false;
	new_command->heredoc_files = NULL;
	new_command->heredoc_count = 0;
	new_command->out_file = NULL;
	new_command->is_append = false;
	new_command->next = NULL;
	return (new_command);
}

void	add_argument(t_command *command, char *new_argument)
{
	size_t	count;
	size_t	i;
	char	**new_args;

	count = 0;
	if (command->args)
	{
		while (command->args[count])
			count++;
	}
	new_args = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		syntax_error("new_args failed!"); // todo
	i = 0;
	while (i < count)
	{
		new_args[i] = command->args[i];
		i++;
	}
	new_args[count] = ft_strdup(new_argument);
	if (!new_args[count])
		free(new_args);                      // Kiki add this line
	syntax_error("new_args strdup failed."); // todo
	new_args[count + 1] = NULL;
	free(command->args);
	command->args = new_args;
}

void	set_redirection(t_command *command, t_token_type token_type,
		char *filename)
{
	if (token_type == TOKEN_REDIR_IN)
	{
		command->input_file = ft_strdup(filename);
		command->is_heredoc = false;
	}
	else if (token_type == TOKEN_HEREDOC)
	{
		command->input_file = ft_strdup(filename);
		command->heredoc_files = realloc(command->heredoc_files, sizeof(char *)
				* (command->heredoc_count + 1));
		if (!command->heredoc_files)
			exit(EXIT_FAILURE); // to do
		command->heredoc_files[command->heredoc_count] = ft_strdup(filename);
		command->heredoc_count++;
		command->is_heredoc = true;
	}
	else if (token_type == TOKEN_REDIR_OUT)
	{
		command->out_file = ft_strdup(filename);
		command->is_append = false;
	}
	else if (token_type == TOKEN_APPEND)
	{
		command->out_file = ft_strdup(filename);
		command->is_append = true;
	}
}

bool	is_redirection_syntax_error(t_token *current_token,
		t_token_type current_type)
{
	t_token	*next_token;

	next_token = current_token->next;
	if (current_type == TOKEN_REDIR_IN || current_type == TOKEN_APPEND
		|| current_type == TOKEN_REDIR_OUT || current_type == TOKEN_HEREDOC)
	{
		if (next_token == NULL || next_token->type != TOKEN_WORD)
			return (true);
	}
	return (false);
}

bool	is_syntax_error(t_token *token_list)
{
	t_token	*former_token;
	t_token	*current_token;

	former_token = NULL;
	current_token = token_list;
	while (current_token->value && current_token->type != TOKEN_EOF)
	{
		if (current_token->type == TOKEN_PIPE && (former_token == NULL
				|| current_token->next->type == TOKEN_PIPE))
			return (fprintf(stdout, "pipe syntax error!\n"), true); // todo
		if (is_redirection_syntax_error(current_token, current_token->type))
			return (fprintf(stdout, "readirection syntax error!\n"), true);
		// todo
		former_token = current_token;
		current_token = current_token->next;
	}
	return (false);
}

t_command	*parser(t_token *token_list)
{
	t_command		*head_command;
	t_command		*current_command;
	t_token_type	type;

	if (is_syntax_error(token_list))
		return (NULL);
	head_command = create_command_node();
	current_command = head_command;
	while (token_list && token_list->type != TOKEN_EOF)
	{
		type = token_list->type;
		if (type == TOKEN_WORD)
			add_argument(current_command, token_list->value);
		else if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
			|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		{
			set_redirection(current_command, type, token_list->next->value);
			token_list = token_list->next;
		}
		else if (type == TOKEN_PIPE)
		{
			current_command->next = create_command_node();
			if (!current_command->next)
			{
				fprintf(stderr, "memory allocation failed for new command\n");
				return (NULL);
			}
			current_command = current_command->next;
		}
		token_list = token_list->next;
	}
	return (head_command);
}
