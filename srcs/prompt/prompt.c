/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:32:43 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/03/19 14:33:23 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_current_directory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("getcwd() error");
		exit(EXIT_FAILURE); // fix how to exit
	}
	return (cwd);
}

#define COLOR_CYAN "\033[36m"
#define COLOR_RESET "\033[0m"

char	*prompt(void)
{
	char	*input;
	char	*cwd;
	char	*prompt_str;
	size_t	prompt_len;

	cwd = get_current_directory();
	prompt_len = strlen("MINISHELL:") + strlen(cwd) + strlen("$> ") + 1;
	prompt_str = malloc(prompt_len);
	if (!prompt_str)
	{
		free(cwd);
		return (NULL);
	}
	snprintf(prompt_str, prompt_len, "MINISHELL:%s$ ", cwd);
	free(cwd);
	input = readline(prompt_str);
	free(prompt_str);
	if (!input)
	{
		printf("exit\n");
		return (NULL);
	}
	if (*input)
		add_history(input);
	if (strcmp(input, "exit") == 0)
		return (NULL);
	return (input);
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
