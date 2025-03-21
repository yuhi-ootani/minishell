/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:32:43 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/03/21 17:13:28 by oyuhi            ###   ########.fr       */
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
	if (!input)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}

char	*get_input(t_minishell *shell, bool interactive_mode)
{
	char	*input_line;

	if (!interactive_mode)
	{
		input_line = ft_get_next_line(STDIN_FILENO);
		if (input_line && strncmp(input_line, "#", 1) == 0)
		{
			free(input_line);
			input_line = ft_get_next_line(STDIN_FILENO);
		}
		if (!input_line)
			shell->exit_status = EXIT_FAILURE;
		return (input_line);
	}
	else
		return (prompt(shell));
}

// char	*prompt(void)
// {
// 	char	*input;
// 	char	*cwd;

// 	while (1)
// 	{
// 		cwd = get_current_directory();
// 		printf("🐾 %s 🐾 ", cwd);
// 		free(cwd);
// 		input = readline("$> ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (*input)
// 			add_history(input);
// 		if (strcmp(input, "exit") == 0)
// 			break ;
// 		return (input);
// 	}
// 	return (NULL);
// }
