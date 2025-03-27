/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:07:23 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/26 16:03:30 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_redirection_type(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_APPEND
		|| type == TOKEN_REDIR_OUT || type == TOKEN_HEREDOC);
}

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
	// t_redirection
	new_command->infiles = NULL;
	new_command->infile_count = 0;
	new_command->outfiles = NULL;
	new_command->outfile_count = 0;
	// new_command->input_file = NULL;
	// new_command->is_heredoc = false;
	// new_command->heredoc_files = NULL;
	// new_command->heredoc_count = 0;
	// new_command->out_file = NULL;
	// new_command->is_append = false;
	new_command->next = NULL;
	return (new_command);
}

bool	add_argument(t_minishell *shell, t_command *cmd, char *new_arg)
{
	size_t	count;
	char	**new_args;
	size_t	old_size;
	size_t	new_size;

	// size_t	i;
	count = ft_array_count_str(cmd->args);
	old_size = (count + 1) * sizeof(char *);
	new_size = (count + 2) * sizeof(char *);
	new_args = (char **)ft_realloc(cmd->args, old_size, new_size);
	if (!new_args)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	new_args[count] = ft_strdup(new_arg);
	if (!new_arg[count])
	{
		free(new_args);
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	new_args[count + 1] = NULL;
	cmd->args = new_args;
	return (true);
	// new_args = (char **)malloc(sizeof(char *) * (count + 2));
	// if (!new_args)
	// 	syntax_error("new_args failed!"); // todo
	// i = 0;
	// while (i < count)
	// {
	// 	new_args[i] = cmd->args[i];
	// 	i++;
	// }
	// new_args[count] = ft_strdup(new_argument);
	// if (!new_args[count])
	// 	syntax_error("new_args strdup failed."); // todo
	// new_args[count + 1] = NULL;
	// free(cmd->args);
	// cmd->args = new_args;
}

t_redirection	*add_redirection_file(t_minishell *shell, t_token *token,
		t_redirection *old_files, size_t old_count)
{
	size_t			old_size;
	size_t			new_size;
	t_redirection	*new_files;
	char			*filename;

	filename = token->next->value;
	old_size = (old_count + 1) * sizeof(t_redirection);
	new_size = (old_count + 2) * sizeof(t_redirection);
	new_files = (t_redirection *)ft_realloc(old_files, old_size, new_size);
	if (!new_files)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	new_files[old_count].filename = ft_strdup(filename);
	if (!new_files[old_count].filename)
	{
		free(new_files);
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	new_files[old_count].type = token->type;
	new_files[old_count + 1].filename = NULL;
	return (new_files);
}

bool	set_redirection(t_minishell *shell, t_command *cmd, t_token *tokens)
{
	// t_redirection
	if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_HEREDOC)
	{
		cmd->infiles = add_redirection_file(shell, tokens, cmd->infiles,
				cmd->infile_count);
		if (!cmd->infiles)
			return (false);
		cmd->infile_count++;
	}
	else if (tokens->type == TOKEN_REDIR_OUT || tokens->type == TOKEN_APPEND)
	{
		cmd->outfiles = add_redirection_file(shell, tokens, cmd->outfiles,
				cmd->outfile_count);
		if (!cmd->outfiles)
			return (false);
		cmd->outfile_count++;
	}
	return (true);
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

t_command	*parser(t_minishell *shell, t_token *tokens)
{
	if (is_syntax_error(shell, tokens))
		return (NULL);
	return (convert_token_into_cmd(shell, tokens));
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
