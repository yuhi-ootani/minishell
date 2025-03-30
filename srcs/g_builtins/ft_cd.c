/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:16:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/24 17:27:32 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_cd(t_minishell *shell)
{
	char		*path;
	t_command	*cmd;

	cmd = shell->commands;
	if (!cmd->args[1])
	{
		path = getenv("HOME");
		if (!path || *path == '\0')
		{
			ft_fprintf(2, "cd: HOME not set\n");
			return ; // to do;
		}
	}
	else
		path = cmd->args[1];
	if (chdir(path) != 0)
	{
		ft_fprintf(2, "cd: %s: %s\n", path, strerror(errno));
		return ; // to do;
	}
}
