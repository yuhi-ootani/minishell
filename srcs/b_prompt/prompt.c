/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:32:43 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/03/30 14:23:21 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_current_directory(t_minishell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		shell->exit_status = EXIT_FAILURE;
	}
	return (cwd);
}

char	*get_prompt_str(t_minishell *shell)
{
	char	*cwd;
	char	*result;

	cwd = get_current_directory(shell);
	if (!cwd)
		return (NULL);
	result = ft_strjoin_three("MINISHELL:", cwd, "$> ");
	if (!result)
		shell->exit_status = EXIT_FAILURE;
	free(cwd);
	return (result);
}

char	*prompt(t_minishell *shell)
{
	char	*input;
	char	*prompt_str;

	prompt_str = get_prompt_str(shell);
	if (!prompt_str)
		return (NULL);
	input = readline(prompt_str);
	free(prompt_str);
	if (!input && g_signal)
		return (NULL);
	if (g_signal)
		shell->exit_status = 130;
	if (input && *input)
		add_history(input);
	return (input);
}

char	*get_uncomment_line(void)
{
	char	*result;

	while (1)
	{
		result = ft_get_next_line(STDIN_FILENO);
		if (result && ft_strncmp(result, "#", 1) == 0)
		{
			free(result);
		}
		else
			return (result);
	}
}

char	*get_input(t_minishell *shell, bool interactive_mode)
{
	char	*input_line;

	if (!interactive_mode)
	{
		input_line = get_uncomment_line();
		if (!input_line)
		{
			free_shell(shell);
			if (errno)
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		return (input_line);
	}
	else
	{
		input_line = prompt(shell);
		return (input_line);
	}
}
