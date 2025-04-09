/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:11:54 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 12:51:13 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	handle_unset_path_error(t_minishell *shell, const char *cmd_str)
// {
// 	ft_fprintf(STDERR_FILENO, "MINISHELL: %s: No such file or directory\n",
// 		cmd_str);
// 	free_shell(shell);
// 	exit(127);
// }

void	cleanup_and_exit_failure(t_minishell *shell, t_exec *exec_info)
{
	if (exec_info)
	{
		if (exec_info->input_fd != -1)
			close(exec_info->input_fd);
		if (exec_info->pipe_fds[0] != -1)
			close(exec_info->pipe_fds[0]);
		if (exec_info->pipe_fds[1] != -1)
			close(exec_info->pipe_fds[1]);
	}
	free_shell(shell);
	exit(EXIT_FAILURE);
}

// char	**get_splited_path(t_minishell *shell, const char *cmd_str)
// {
// 	char	*env_path;
// 	char	**splited_path;

// 	if (!get_env_value(shell, "PATH", &env_path))
// 		return (NULL);
// 	if (!env_path)
// 	{
// 		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: No such file or directory\n",
// 			cmd_str);
// 		free_shell(shell);
// 		exit(127);
// 	}
// 	splited_path = ft_split(env_path, ":");
// 	free(env_path);
// 	return (splited_path);
// }

// char	*search_command_in_path(t_minishell *shell, const char *command)
// {
// 	char	**splited_path;
// 	int		i;
// 	char	*full_command_path;

// 	splited_path = get_splited_path(shell, command);
// 	if (!splited_path)
// 		cleanup_and_exit_failure(shell, NULL);
// 	i = 0;
// 	while (splited_path && splited_path[i])
// 	{
// 		full_command_path = ft_strjoin_three(splited_path[i], "/", command);
// 		if (!full_command_path)
// 		{
// 			ft_array_free(splited_path);
// 			cleanup_and_exit_failure(shell, NULL);
// 		}
// 		if (access(full_command_path, F_OK | X_OK) == 0)
// 			return (ft_array_free(splited_path), full_command_path);
// 		free(full_command_path);
// 		i++;
// 	}
// 	ft_array_free(splited_path);
// 	return (NULL);
// }

// char	*get_cmd_path(t_minishell *shell, const char *cmd_str)
// {
// 	char	*cmd_path;

// 	if (ft_strchr(cmd_str, '/'))
// 	{
// 		cmd_path = ft_strdup(cmd_str);
// 		if (!cmd_path)
// 			cleanup_and_exit_failure(shell, NULL);
// 	}
// 	else
// 	{
// 		cmd_path = search_command_in_path(shell, cmd_str);
// 		if (!cmd_path)
// 		{
// 			ft_fprintf(STDERR_FILENO, "MINISHELL: %s: command not found\n",
// 				cmd_str);
// 			free(cmd_path);
// 			free_shell(shell);
// 			exit(127);
// 		}
// 	}
// 	return (cmd_path);
// }

// void	execute_external_command(t_minishell *shell, t_command *cmd)
// {
// 	char	*command_path;
// 	char	**envp_array;

// 	envp_array = build_envp_array(shell);
// 	command_path = get_cmd_path(shell, cmd->args[0]);
// 	if (execve(command_path, cmd->args, envp_array) == -1)
// 	{
// 		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: %s\n", cmd->args[0],
// 			strerror(errno));
// 		ft_array_free(envp_array);
// 		free(command_path);
// 		free_shell(shell);
// 		if (errno == EACCES)
// 			exit(126);
// 		else
// 			exit(errno);
// 	}
// }

bool	setup_infile(t_minishell *shell, t_exec *exec_info,
		t_command *current_cmd)
{
	if (exec_info->input_fd != STDIN_FILENO)
	{
		if (shell->commands->infile_count == 0)
		{
			if (dup2(exec_info->input_fd, STDIN_FILENO) == -1)
				cleanup_and_exit_failure(shell, exec_info);
		}
		close(exec_info->input_fd);
		exec_info->input_fd = -1;
	}
	if (!input_redirection(shell, current_cmd))
		return (false);
	return (true);
}

bool	setup_outfile(t_minishell *shell, t_exec *exec_info,
		t_command *current_cmd)
{
	if (current_cmd->next)
	{
		if (shell->commands->outfile_count == 0)
		{
			if (dup2(exec_info->pipe_fds[1], STDOUT_FILENO) == -1)
				cleanup_and_exit_failure(shell, exec_info);
		}
		exec_info->input_fd = exec_info->pipe_fds[0];
		close(exec_info->pipe_fds[1]);
		exec_info->pipe_fds[1] = -1;
	}
	if (!output_redirection(shell, current_cmd))
		return (false);
	return (true);
}

void	close_unsed_fds(t_minishell *shell)
{
	close(shell->original_stdin);
	shell->original_stdin = -1;
	close(shell->original_stdout);
	shell->original_stdout = -1;
}

void	execute_child_process(t_minishell *shell, t_exec *exec_info,
		t_command *current_cmd)
{
	int	exit_status;

	setup_signals_child();
	exit_status = 0;
	close_unsed_fds(shell);
	if (!setup_infile(shell, exec_info, current_cmd) || !setup_outfile(shell,
			exec_info, current_cmd))
		cleanup_and_exit_failure(shell, exec_info);
	if (!current_cmd->args || !current_cmd->args[0][0])
	{
		free_shell(shell);
		exit(EXIT_SUCCESS);
	}
	if (current_cmd->args[0])
		exec_info->builtin_id = is_builtin(current_cmd->args[0]);
	if (exec_info->builtin_id != NOT_BUILTIN)
	{
		exit_status = exec_info->builtins[exec_info->builtin_id](shell);
		free_shell(shell);
		exit(exit_status);
	}
	else
		execute_external_command(shell, current_cmd);
}
