/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:13 by oyuhi             #+#    #+#             */
/*   Updated: 2025/02/26 15:18:54 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_buildin_cmd	is_builtin(char *cmd_str)
{
	if (strcmp(cmd_str, "echo") == 0)
		return (ECHO);
	else if (strcmp(cmd_str, "cd") == 0)
		return (CD);
	else if (strcmp(cmd_str, "pwd") == 0)
		return (PWD);
	else if (strcmp(cmd_str, "export") == 0)
		return (EXPORT);
	else if (strcmp(cmd_str, "unset") == 0)
		return (UNSET);
	else if (strcmp(cmd_str, "env") == 0)
		return (ENV);
	else if (strcmp(cmd_str, "exit") == 0)
		return (EXIT);
	else
		return (NOT_BUILDIN);
}

void	single_command_executor(t_command *command)
{
	static int (*builtin_funcs[])(t_command *) = {ft_echo, ft_cd, ft_pwd,
		ft_export, ft_unset, ft_env, ft_exit};

	t_buildin_cmd buildin_cmd_nbr = is_builtin(command->args[0]);
	if (buildin_cmd_nbr != NOT_BUILDIN)
		builtin_funcs[buildin_cmd_nbr](command); // Execute the function
	else
		printf("Command not found: %s\n", command->args[0]);
}