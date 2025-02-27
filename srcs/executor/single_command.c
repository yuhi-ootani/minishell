/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:13 by oyuhi             #+#    #+#             */
/*   Updated: 2025/02/26 17:41:44 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_buildin_cmd	is_builtin(char *command_str)
{
	if (strcmp(command_str, "echo") == 0)
		return (FT_ECHO);
	else if (strcmp(command_str, "cd") == 0)
		return (FT_CD);
	else if (strcmp(command_str, "pwd") == 0)
		return (FT_PWD);
	else if (strcmp(command_str, "export") == 0)
		return (FT_EXPORT);
	else if (strcmp(command_str, "unset") == 0)
		return (FT_UNSET);
	else if (strcmp(command_str, "env") == 0)
		return (FT_ENV);
	else if (strcmp(command_str, "exit") == 0)
		return (FT_EXIT);
	else
		return (FT_NOT_BUILDIN);
}

void	single_command_executor(t_command *command)
{
	static int (*builtin_funcs[])(t_command *) = {ft_echo, ft_cd, ft_pwd,
		ft_export, ft_unset, ft_env, ft_exit};

	t_buildin_cmd buildin_cmd_nbr = is_builtin(command->args[0]);
	if (buildin_cmd_nbr != FT_NOT_BUILDIN)
		builtin_funcs[buildin_cmd_nbr](command); // Execute the function
	else
		printf("Command not found: %s\n", command->args[0]);
}