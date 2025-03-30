/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:07:23 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/30 15:15:51 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*create_command_node(t_minishell *shell)
{
	t_command	*new_command;

	new_command = (t_command *)malloc(sizeof(t_command));
	if (!new_command)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	new_command->args = NULL;
	new_command->infiles = NULL;
	new_command->infile_count = 0;
	new_command->outfiles = NULL;
	new_command->outfile_count = 0;
	new_command->next = NULL;
	return (new_command);
}

bool	add_token_content_to_cmd(t_minishell *shell, t_token *tokens,
		t_command *cmd)
{
	if (tokens->type == TOKEN_WORD)
	{
		if (!add_argument(shell, cmd, tokens->value))
			return (false);
	}
	else if (is_redirection_type(tokens->type))
	{
		if (!set_redirection(shell, cmd, tokens))
			return (false);
	}
	return (true);
}

t_token	*move_to_next_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (is_redirection_type(tokens->type))
	{
		tokens = tokens->next;
		if (tokens)
			tokens = tokens->next;
		return (tokens);
	}
	return (tokens->next);
}

t_command	*convert_token_into_cmd(t_minishell *shell, t_token *tokens)
{
	t_command	*head_cmd;
	t_command	*current_command;

	head_cmd = create_command_node(shell);
	if (!head_cmd)
		return (NULL);
	current_command = head_cmd;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			current_command->next = create_command_node(shell);
			if (!current_command->next)
				return (free_commands(head_cmd), NULL);
			current_command = current_command->next;
		}
		else
		{
			if (!add_token_content_to_cmd(shell, tokens, current_command))
				return (free_commands(head_cmd), NULL);
		}
		tokens = move_to_next_token(tokens);
	}
	return (head_cmd);
}
// void	add_file_back(t_file **head, t_file *new_file)
// {
// 	t_file	*current;

// 	if (!*head)
// 	{
// 		*head = new_file;
// 		return ;
// 	}
// 	current = *head;
// 	while (current->next)
// 		current = current->next;
// 	current->next = new_file;
// }

// bool	set_input_redirection(t_minishell *shell, t_command *cmd,
// 		t_token *tokens)
// {
// 	t_file *new_file;

// 	new_file = create_redir_file(shell, tokens);
// 	if (!new_file)
// 		return (false);
// 	add_file_back(&cmd->infiles, new_file);
// 	cmd->infile_count++;
// 	return (true);
// }

// MUKASHINO YATSU

// bool	set_input_redirection(t_minishell *shell, t_command *cmd,
// 		char *filename)
// {
// 	cmd->is_heredoc = false;
// 	cmd->input_file = ft_strdup(filename);
// 	if (cmd->input_file)
// 	{
// 		shell->exit_status = EXIT_FAILURE;
// 		return (false);
// 	}
// 	return (true);
// }

// bool	set_input_heredoc(t_minishell *shell, t_command *cmd, char *filename)
// {
// 	size_t	old_size;
// 	size_t	new_size;

// 	cmd->input_file = ft_strdup(filename); // may not update infile
// 	old_size = sizeof(char *) * (cmd->heredoc_count + 1);
// 	new_size = sizeof(char *) * (cmd->heredoc_count + 2);
// 	cmd->heredoc_files = ft_realloc(cmd->heredoc_files, old_size, new_size);
// 	if (!cmd->heredoc_files)
// 	{
// 		shell->exit_status = EXIT_FAILURE;
// 		return (false);
// 	}
// 	cmd->heredoc_files[cmd->heredoc_count] = ft_strdup(filename);
// 	if (!cmd->heredoc_files[cmd->heredoc_count])
// 	{
// 		shell->exit_status = EXIT_FAILURE;
// 		return (false);
// 	}
// 	cmd->heredoc_count++;
// 	cmd->heredoc_files[cmd->heredoc_count] = NULL;
// 	cmd->is_heredoc = true;
// 	return (true);
// }
