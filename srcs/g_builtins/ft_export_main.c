/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:45:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 18:59:18 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖    ▗▖  ▗▖▗▄▄▄▖▗▖ ▗▖    ▗▄▄▄▖▗▖  ▗▖▗▖  ▗▖
// ▐▌   ▐▌     █      ▐▛▚▖▐▌▐▌   ▐▌ ▐▌    ▐▌   ▐▛▚▖▐▌▐▌  ▐▌
//  ▝▀▚▖▐▛▀▀▘  █      ▐▌ ▝▜▌▐▛▀▀▘▐▌ ▐▌    ▐▛▀▀▘▐▌ ▝▜▌▐▌  ▐▌
// ▗▄▄▞▘▐▙▄▄▖  █      ▐▌  ▐▌▐▙▄▄▖▐▙█▟▌    ▐▙▄▄▖▐▌  ▐▌ ▝▚▞▘

static t_env	*get_env_if_exist(t_env *copied_env, const char *new_name)
{
	t_env	*env;

	env = copied_env;
	while (env)
	{
		if (ft_strcmp(env->name, new_name) == 0)
		{
			return (env);
		}
		env = env->next;
	}
	return (NULL);
}

int	update_env_value(t_env *env, const char *new_value, bool append)
{
	char	*tmp_value;

	if (append && env->value)
		tmp_value = ft_strjoin(env->value, new_value);
	else
		tmp_value = ft_strdup(new_value);
	if (!tmp_value)
		return (EXIT_FAILURE);
	free(env->value);
	env->value = tmp_value;
	return (EXIT_SUCCESS);
}

static int	set_env_value(t_env *copied_env, const char *new_name,
		const char *new_value, bool append)
{
	t_env	*new_env;

	new_env = get_env_if_exist(copied_env, new_name);
	if (new_env)
	{
		return (update_env_value(new_env, new_value, append));
	}
	else
	{
		new_env = create_new_env_util(new_name, new_value, NULL);
		if (!new_env)
			return (EXIT_FAILURE);
		env_add_back_util(&copied_env, new_env);
		return (EXIT_SUCCESS);
	}
}

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