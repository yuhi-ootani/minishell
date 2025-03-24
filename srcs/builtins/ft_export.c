/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:45:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/24 17:28:44 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖    ▗▖  ▗▖▗▄▄▄▖▗▖ ▗▖    ▗▄▄▄▖▗▖  ▗▖▗▖  ▗▖
// ▐▌   ▐▌     █      ▐▛▚▖▐▌▐▌   ▐▌ ▐▌    ▐▌   ▐▛▚▖▐▌▐▌  ▐▌
//  ▝▀▚▖▐▛▀▀▘  █      ▐▌ ▝▜▌▐▛▀▀▘▐▌ ▐▌    ▐▛▀▀▘▐▌ ▝▜▌▐▌  ▐▌
// ▗▄▄▞▘▐▙▄▄▖  █      ▐▌  ▐▌▐▙▄▄▖▐▙█▟▌    ▐▙▄▄▖▐▌  ▐▌ ▝▚▞▘

static bool	update_env_value_if_exists(t_env **copied_env, const char *new_name,
		const char *new_value, bool append)
{
	t_env	*tmp;
	char	*new_allocated_value;

	tmp = *copied_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, new_name) == 0)
		{
			if (append && tmp->value)
				new_allocated_value = ft_strjoin(tmp->value, new_value);
			else
				new_allocated_value = ft_strdup(new_value);
			if (!new_allocated_value)
				return (false); // handle malloc error
			free(tmp->value);
			tmp->value = new_allocated_value;
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

static void	set_env_value(t_env **copied_env, const char *new_name,
		const char *new_value, bool append)
{
	t_env	*new_env;

	if (update_env_value_if_exists(copied_env, new_name, new_value, append))
		return ;
	new_env = create_new_env_util(new_name, new_value, NULL);
	if (!new_env)
		return ; // todo
	env_add_back_util(copied_env, new_env);
}

//  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖    ▗▖  ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄▖    ▗▖  ▗▖ ▗▄▖ ▗▖   ▗▖ ▗▖▗▄▄▄▖
// ▐▌   ▐▌     █      ▐▛▚▖▐▌▐▌ ▐▌▐▛▚▞▜▌▐▌       ▐▌  ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌▐▌
// ▐▌▝▜▌▐▛▀▀▘  █      ▐▌ ▝▜▌▐▛▀▜▌▐▌  ▐▌▐▛▀▀▘    ▐▌  ▐▌▐▛▀▜▌▐▌   ▐▌ ▐▌▐▛▀▀▘
// ▝▚▄▞▘▐▙▄▄▖  █      ▐▌  ▐▌▐▌ ▐▌▐▌  ▐▌▐▙▄▄▖     ▝▚▞▘ ▐▌ ▐▌▐▙▄▄▖▝▚▄▞▘▐▙▄▄▖

static size_t	skip_operands(const char *arg, size_t i, bool *append)
{
	if (arg[i] == '+' && arg[i + 1] == '=')
	{
		*append = true;
		return (i + 2);
	}
	else if (arg[i] == '=')
		return (i + 1);
	return (i);
}

static void	get_name_and_value(char **name, char **value, char *arg,
		bool *append)
{
	size_t	i;

	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	*name = ft_strndup(arg, i);
	// if name = NULL todo error
	if (arg[i])
	{
		i = skip_operands(arg, i, append);
		if (arg[i])
			*value = ft_strdup(arg + i);
		else
			*value = ft_strdup("");
	}
	else
		*value = NULL;
	// if value = NULL todo error
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
	// if (!ft_strchr(arg, '='))
	// 	return (true);
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

static void	set_new_env_variables(char *arg, t_env **copied_env)
{
	char	*name;
	char	*value;
	bool	append;

	if (is_invalid_arg(arg))
	{
		ft_fprintf(2,
					"export: `"
					"%s': "
					"not a valid idenifier\n",
					arg);
		return ;
	} // todo
	get_name_and_value(&name, &value, arg, &append);
	set_env_value(copied_env, name, value, append);
	free(name);
	free(value);
}

//  ▗▄▄▖▗▄▄▄▖▗▄▖ ▗▄▄▖▗▄▄▄▖
// ▐▌     █ ▐▌ ▐▌▐▌ ▐▌ █
//  ▝▀▚▖  █ ▐▛▀▜▌▐▛▀▚▖ █
// ▗▄▄▞▘  █ ▐▌ ▐▌▐▌ ▐▌ █

void	ft_export(t_minishell *shell)
{
	size_t		i;
	t_command	*cmd;

	cmd = shell->commands;
	if (cmd->args[1])
	{
		i = 1;
		while (cmd->args[i])
		{
			set_new_env_variables(cmd->args[i], &shell->env);
			i++;
		}
	}
	else
		sort_and_print_env(&shell->env);
	return ;
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