/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:16:13 by knemcova          #+#    #+#             */
/*   Updated: 2025/02/26 14:49:04 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int main(int argc, char **argv, char **envp)
// {
//     char input[1024];
//     char *args[10];

//     while (1)
//     {
//         printf("$> ");
//         fgets(input, sizeof(input), stdin);
//         input[strcspn(input, "\n")] = 0;

//         int i = 0;
//         char *token = strtok(input, " ");
//         while (token)
//         {
//             args[i++] = token;
//             token = strtok(NULL, " ");
//         }
//         args[i] = NULL;

//         builtin_commands(args, envp);
//     }
//     return (0);
// }

int	main(int argc, char **argv, char **envp)
{
	char	input[1024];
	char	*args[10];
	int		i;
	char	*token;

	(void)argc;
	(void)argv;
	setup_signals();
	while (1)
	{
		if (g_signal)
		{
			g_signal = 0;
			printf("\n$> ");
			fflush(stdout);
		}
		printf("$> ");
		if (!fgets(input, sizeof(input), stdin))
		{
			break ;
		}
		input[strcspn(input, "\n")] = 0;
		i = 0;
		token = strtok(input, " ");
		while (token)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;
		builtin_commands(args, envp);
	}
	return (0);
}
