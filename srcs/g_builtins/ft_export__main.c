/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export__main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:45:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/10 10:34:23 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	set_new_env_variable(char *arg, t_env *copied_env)
{
	char	*name;
	char	*value;
	bool	append;
	int		exit_status;

	if (!get_name_and_value(arg, &name, &value, &append))
		return (EXIT_FAILURE);
	exit_status = set_env_value(copied_env, name, value, append);
	if (name)
		free(name);
	if (value)
		free(value);
	return (exit_status);
}

bool	is_invalid_arg(char *arg)
{
	size_t	i;

	i = 0;
	if (!arg || arg[0] == '\0')
		return (true);
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (true);
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
			break ;
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (true);
		i++;
	}
	return (false);
}

int	ft_export(t_minishell *shell)
{
	size_t		i;
	t_command	*cmd;
	int			exit_status;

	cmd = shell->commands;
	if (cmd->args[1])
	{
		i = 1;
		while (cmd->args[i])
		{
			if (is_invalid_arg(cmd->args[i]))
			{
				ft_fprintf(2, "MINISHELL: export: %s: not a valid idenifier\n",
					cmd->args[i]);
				exit_status = EXIT_FAILURE;
			}
			else
				exit_status = set_new_env_variable(cmd->args[i], shell->env);
			i++;
		}
	}
	else
		exit_status = sort_and_print_env(&shell->env);
	return (exit_status);
}

// int	main(void)
// {
// 	char	*input;

// 	while ((input = readline("> ")))
// 	{
// 		printf("%d\n", is_invalid_arg(input));
// 		free(input);
// 	}
// 	return (0);
// }