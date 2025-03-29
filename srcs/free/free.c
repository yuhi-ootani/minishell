/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:37:45 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/29 13:41:22 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_command(t_command *cmd)
{
	size_t	i;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	// t_redirection
	i = 0;
	if (cmd->infiles)
	{
		while (i < cmd->infile_count)
		{
			free(cmd->infiles[i].filename);
			i++;
		}
		free(cmd->infiles);
	}
	i = 0;
	if (cmd->outfiles)
	{
		while (i < cmd->outfile_count)
		{
			free(cmd->outfiles[i].filename);
			i++;
		}
		free(cmd->outfiles);
	}
	// if (cmd->input_file)
	// 	free(cmd->input_file);
	// if (cmd->out_file)
	// 	free(cmd->out_file);
	free(cmd);
}

void	free_commands(t_command *head)
{
	t_command	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free_command(tmp);
	}
}

void	free_shell(t_minishell *shell)
{
	close(shell->original_stdout);
	close(shell->original_stdin);
	free_copied_env(shell->env);
	if (shell->input)
		free(shell->input);
	if (shell->commands)
		free_commands(shell->commands);
}

void	free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
