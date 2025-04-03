/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:37:45 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 14:27:14 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_shell(t_minishell *shell)
{
	if (shell->original_stdout > -1)
		close(shell->original_stdout);
	if (shell->original_stdin > -1)
		close(shell->original_stdin);
	free_copied_env(shell->env);
	if (shell->input)
		free(shell->input);
	if (shell->commands)
		free_all_cmds(shell->commands);
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

void	free_env(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = temp;
	}
}

//free_env is not used anywhere

void	free_copied_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}
