/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:41:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/15 16:36:19 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_pwd(t_minishell *shell)
{
	char	cwd[PATH_MAX];

	(void)shell;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, 1);
	}
	else
	{
		perror("pwd");
	}
}
