/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:07:23 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/25 09:37:23 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	syntax_error(const char *statement)
{
	ft_printf("%s\n", statement);
	ft_fprintf(2, "Minishell: syntax error\n");
	exit(EXIT_FAILURE);
}

t_command	*create_command_node(void)
{
	t_command	*new_command;

	new_command = (t_command *)malloc(sizeof(t_command));
	if (!new_command)
		syntax_error("create_command_node");
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

void	add_argument(t_command *cmd, char *new_argument)
{
	size_t	count;
	size_t	i;
	char	**new_args;

	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	new_args = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		syntax_error("new_args failed!"); // todo
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_strdup(new_argument);
	if (!new_args[count])
		syntax_error("new_args strdup failed."); // todo
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

void	set_redirection(t_command *cmd, t_token_type token_type, char *filename)
{
	size_t	old_size;
	size_t	new_size;

	if (token_type == TOKEN_REDIR_IN)
	{
		cmd->input_file = ft_strdup(filename);
		cmd->is_heredoc = false;
	}
	else if (token_type == TOKEN_HEREDOC)
	{
		cmd->input_file = ft_strdup(filename);
		old_size = sizeof(char *) * cmd->heredoc_count;
		new_size = sizeof(char *) * (cmd->heredoc_count + 1);
		cmd->heredoc_files = ft_realloc(cmd->heredoc_files, old_size, new_size);
		if (!cmd->heredoc_files)
			exit(EXIT_FAILURE); // to do
		cmd->heredoc_files[cmd->heredoc_count] = ft_strdup(filename);
		cmd->heredoc_count++;
		cmd->is_heredoc = true;
	}
	else if (token_type == TOKEN_REDIR_OUT)
	{
		cmd->out_file = ft_strdup(filename);
		cmd->is_append = false;
	}
	else if (token_type == TOKEN_APPEND)
	{
		cmd->out_file = ft_strdup(filename);
		cmd->is_append = true;
	}
}

bool	is_redirection_syntax_error(t_minishell *shell, t_token *current_token)
{
	t_token			*next_token;
	t_token_type	current_type;

	current_type = current_token->type;
	next_token = current_token->next;
	if (current_type == TOKEN_REDIR_IN || current_type == TOKEN_APPEND
		|| current_type == TOKEN_REDIR_OUT || current_type == TOKEN_HEREDOC)
	{
		if (next_token == NULL || next_token->type != TOKEN_WORD)
		{
			shell->exit_status = 2;
			if (next_token && next_token->value)
				ft_fprintf(STDERR_FILENO,
					"syntax error near unexpected token `%s'\n",
					next_token->value);
			else
				ft_fprintf(STDERR_FILENO,
					"syntax error near unexpected token `newline'\n");
			return (true);
		}
	}
	return (false);
}

bool	is_pipe_syntax_error(t_minishell *shell, t_token *current_token,
		t_token *prev_token)
{
	t_token_type	current_type;

	current_type = current_token->type;
	if (current_type == TOKEN_PIPE)
	{
		if (prev_token == NULL || current_token->next->type == TOKEN_PIPE)
		{
			shell->exit_status = 2;
			ft_fprintf(STDERR_FILENO,
				"syntax error near unexpected token `|'\n");
			return (true);
		}
		if (current_token->next == NULL)
		{
			shell->exit_status = 2;
			ft_fprintf(STDERR_FILENO,
				"syntax error near unexpected token `newline'\n");
			return (true);
		}
	}
	return (false);
}

bool	is_syntax_error(t_minishell *shell, t_token *tokens)
{
	t_token	*prev_token;
	t_token	*current_token;

	prev_token = NULL;
	current_token = tokens;
	while (current_token->value && current_token->type != TOKEN_EOF)
	{
		if (is_pipe_syntax_error(shell, current_token, prev_token))
			return (true);
		if (is_redirection_syntax_error(shell, current_token))
			return (true);
		prev_token = current_token;
		current_token = current_token->next;
	}
	return (false);
}

t_command	*parser(t_minishell *shell, t_token *tokens)
{
	t_command		*head_command;
	t_command		*current_command;
	t_token_type	type;

	if (is_syntax_error(shell, tokens))
		return (NULL);
	head_command = create_command_node();
	current_command = head_command;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		type = tokens->type;
		if (type == TOKEN_WORD)
			add_argument(current_command, tokens->value);
		else if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
			|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		{
			set_redirection(current_command, type, tokens->next->value);
			tokens = tokens->next;
		}
		else if (type == TOKEN_PIPE)
		{
			current_command->next = create_command_node();
			if (!current_command->next)
			{
				ft_fprintf(2, "memory allocation failed for new command\n");
				return (NULL);
			}
			current_command = current_command->next;
		}
		tokens = tokens->next;
	}
	return (head_command);
}
