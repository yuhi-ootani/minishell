/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:32:43 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/03/03 12:11:36 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_current_directory(void)
{
	char	*cwd;

	// If buf is NULL, space is allocated as necessary to store the pathname
	// and size is ignored.This space may later be free(3)'d. if (getcwd(cwd,
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("getcwd() error");
		exit(EXIT_FAILURE); // fix how to exit
	}
	return (cwd);
}

char	*prompt(void)
{
	char *input;
	char *cwd;

	while (1)
	{
		cwd = get_current_directory();
		printf("🐾 %s 🐾 ", cwd);
		free(cwd);
		input = readline("$>");
		if (input == NULL)
			break ;
		else if (strcmp(input, "exit") == 0)
		{
			return (NULL);
		}
		else
		{
			break ;
		}
	}
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
// 		input = readline("$>");
// 		if (input == NULL)
// 			break ;
// 		else if (*input)
// 			add_history(input);
// 		else if (strcmp(input, "exit") == 0)
// 		{
// 			return (NULL);
// 		}
// 		else
// 		{
// 			break ;
// 		}
// 	}
// 	return (input);
// }
