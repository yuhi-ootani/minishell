/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:49:13 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 13:04:25 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**get_splited_path(t_minishell *shell, const char *cmd_str)
{
	char	*env_path;
	char	**splited_path;

	if (!get_env_value(shell, "PATH", &env_path))
		return (NULL);
	if (!env_path)
	{
		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: No such file or directory\n",
			cmd_str);
		free_shell(shell);
		exit(127);
	}
	splited_path = ft_split(env_path, ":");
	free(env_path);
	return (splited_path);
}

char	*search_command_in_path(t_minishell *shell, const char *command)
{
	char	**splited_path;
	int		i;
	char	*full_command_path;

	splited_path = get_splited_path(shell, command);
	if (!splited_path)
		cleanup_and_exit_failure(shell, NULL);
	i = 0;
	while (splited_path && splited_path[i])
	{
		full_command_path = ft_strjoin_three(splited_path[i], "/", command);
		if (!full_command_path)
		{
			ft_array_free(splited_path);
			cleanup_and_exit_failure(shell, NULL);
		}
		if (access(full_command_path, F_OK | X_OK) == 0)
			return (ft_array_free(splited_path), full_command_path);
		free(full_command_path);
		i++;
	}
	ft_array_free(splited_path);
	return (NULL);
}

char	*get_cmd_path(t_minishell *shell, const char *cmd_str)
{
	char	*cmd_path;

	if (ft_strchr(cmd_str, '/'))
	{
		cmd_path = ft_strdup(cmd_str);
		if (!cmd_path)
			cleanup_and_exit_failure(shell, NULL);
	}
	else
	{
		cmd_path = search_command_in_path(shell, cmd_str);
		if (!cmd_path)
		{
			ft_fprintf(STDERR_FILENO, "MINISHELL: %s: command not found\n",
				cmd_str);
			free(cmd_path);
			free_shell(shell);
			exit(127);
		}
	}
	return (cmd_path);
}

void	execute_external_command(t_minishell *shell, t_command *cmd)
{
	char	*command_path;
	char	**envp_array;

	envp_array = build_envp_array(shell);
	command_path = get_cmd_path(shell, cmd->args[0]);
	if (execve(command_path, cmd->args, envp_array) == -1)
	{
		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: %s\n", cmd->args[0],
			strerror(errno));
		ft_array_free(envp_array);
		free(command_path);
		free_shell(shell);
		if (errno == EACCES)
			exit(126);
		else
			exit(errno);
	}
}
