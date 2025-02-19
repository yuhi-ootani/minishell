/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/02/19 16:46:11 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	char	*input;

	printf("Enther commands:\n");
	printf(" 'clear' : Clear command history.\n");
	printf(" 'replace' : Replace current line with a preset message.\n");
	printf(" 'exit' : Exit the program.\n\n");
	while ((input = readline("Prompt >")) != NULL)
	{
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		printf("%s\n", input);
	}
	return (0);
}
