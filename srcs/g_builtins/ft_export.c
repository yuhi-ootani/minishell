/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:45:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 15:04:52 by knemcova         ###   ########.fr       */
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

//  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖    ▗▖  ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄▖    ▗▖  ▗▖ ▗▄▖ ▗▖   ▗▖ ▗▖▗▄▄▄▖
// ▐▌   ▐▌     █      ▐▛▚▖▐▌▐▌ ▐▌▐▛▚▞▜▌▐▌       ▐▌  ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌▐▌
// ▐▌▝▜▌▐▛▀▀▘  █      ▐▌ ▝▜▌▐▛▀▜▌▐▌  ▐▌▐▛▀▀▘    ▐▌  ▐▌▐▛▀▜▌▐▌   ▐▌ ▐▌▐▛▀▀▘
// ▝▚▄▞▘▐▙▄▄▖  █      ▐▌  ▐▌▐▌ ▐▌▐▌  ▐▌▐▙▄▄▖     ▝▚▞▘ ▐▌ ▐▌▐▙▄▄▖▝▚▄▞▘▐▙▄▄▖

static size_t	skip_operands(const char *arg, size_t i)
{
	if (arg[i] == '+' && arg[i + 1] == '=')
	{
		return (i + 2);
	}
	else if (arg[i] == '=')
		return (i + 1);
	return (i);
}

char	*get_value(char *arg, size_t i)
{
	char	*value;

	if (arg[i])
		value = ft_strdup(arg + i);
	else
		value = ft_strdup("");
	return (value);
}

static bool	get_name_and_value(char *arg, char **name, char **value,
		bool *append)
{
	size_t	i;

	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	*name = ft_strndup(arg, i);
	if (!name)
		return (false);
	*append = false;
	if (arg[i] == '+' && arg[i + 1] == '=')
		*append = true;
	i = skip_operands(arg, i);
	*value = NULL;
	if (arg[i])
	{
		*value = get_value(arg, i);
		if (!*value)
			return (free(*name), false);
	}
	return (true);
}

static bool	set_new_env_variable(char *arg, t_env *copied_env)
{
	char	*name;
	char	*value;
	bool	append;
	int		exit_status;

	if (!get_name_and_value(arg, &name, &value, &append))
		return (EXIT_FAILURE);
	printf("kkljfds;lkfjasd;lkfjas;lkfj%s\n", value);
	exit_status = set_env_value(copied_env, name, value, append);
	if (name)
		free(name);
	if (value)
		free(value);
	return (exit_status);
}

// ▗▄▄▖  ▗▄▖ ▗▄▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▖
// ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌   ▐▌ ▐▌
// ▐▛▀▘ ▐▛▀▜▌▐▛▀▚▖ ▝▀▚▖▐▛▀▀▘▐▛▀▚▖
// ▐▌   ▐▌ ▐▌▐▌ ▐▌▗▄▄▞▘▐▙▄▄▖▐▌ ▐▌

static bool	is_invalid_arg(char *arg)
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

//  ▗▄▄▖▗▄▄▄▖▗▄▖ ▗▄▄▖▗▄▄▄▖
// ▐▌     █ ▐▌ ▐▌▐▌ ▐▌ █
//  ▝▀▚▖  █ ▐▛▀▜▌▐▛▀▚▖ █
// ▗▄▄▞▘  █ ▐▌ ▐▌▐▌ ▐▌ █

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