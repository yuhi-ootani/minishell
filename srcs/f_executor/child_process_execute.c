/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_execute.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:26:51 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/10 09:34:56 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**build_envp_array(t_minishell *shell, t_exec *exec_info)
{
	size_t	count;
	char	**envp_array;
	t_env	*tmp;

	count = 0;
	count = count_env_util(shell->env);
	envp_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp_array)
		cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
	count = 0;
	tmp = shell->env;
	while (tmp)
	{
		envp_array[count] = ft_strjoin_three(tmp->name, "=", tmp->value);
		if (!envp_array[count])
			cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
		count++;
		tmp = tmp->next;
	}
	envp_array[count] = NULL;
	return (envp_array);
}

char	**get_splited_path(t_minishell *shell, t_exec *exec_info,
		const char *cmd_str)
{
	char	*env_path;
	char	**splited_path;

	if (!get_env_value_util(shell, "PATH", &env_path))
		return (NULL);
	if (!env_path)
	{
		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: No such file or directory\n",
			cmd_str);
		cleanup_and_exit_child(shell, exec_info, 127);
	}
	splited_path = ft_split(env_path, ":");
	free(env_path);
	return (splited_path);
}

char	*search_command_in_path(t_minishell *shell, t_exec *exec_info,
		const char *command)
{
	char	**splited_path;
	int		i;
	char	*full_command_path;

	splited_path = get_splited_path(shell, exec_info, command);
	if (!splited_path)
		cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
	i = 0;
	while (splited_path && splited_path[i])
	{
		full_command_path = ft_strjoin_three(splited_path[i], "/", command);
		if (!full_command_path)
		{
			ft_array_free(splited_path);
			cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
		}
		if (access(full_command_path, F_OK | X_OK) == 0)
			return (ft_array_free(splited_path), full_command_path);
		free(full_command_path);
		i++;
	}
	ft_array_free(splited_path);
	return (NULL);
}

char	*get_cmd_path(t_minishell *shell, t_exec *exec_info,
		const char *cmd_str)
{
	char	*cmd_path;

	if (ft_strchr(cmd_str, '/'))
	{
		cmd_path = ft_strdup(cmd_str);
		if (!cmd_path)
			cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
	}
	else
	{
		cmd_path = search_command_in_path(shell, exec_info, cmd_str);
		if (!cmd_path)
		{
			ft_fprintf(STDERR_FILENO, "MINISHELL: %s: command not found\n",
				cmd_str);
			free(cmd_path);
			cleanup_and_exit_child(shell, exec_info, 127);
		}
	}
	return (cmd_path);
}

void	execute_external_command(t_minishell *shell, t_exec *exec_info,
		t_command *cmd)
{
	char	*command_path;
	char	**envp_array;

	command_path = get_cmd_path(shell, exec_info, cmd->args[0]);
	envp_array = build_envp_array(shell, exec_info);
	if (execve(command_path, cmd->args, envp_array) == -1)
	{
		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: %s\n", cmd->args[0],
			strerror(errno));
		ft_array_free(envp_array);
		free(command_path);
		if (errno == EACCES)
			cleanup_and_exit_child(shell, exec_info, 126);
		else
			cleanup_and_exit_child(shell, exec_info, errno);
	}
}
