
#include "../../include/minishell.h"

void	no_such_file_or_directory(t_minishell *shell, const char *command)
{
	ft_fprintf(STDERR_FILENO, "MINISHELL: %s: No such file or directory\n",
		command);
	exit(127);
}

char	*search_command_in_path(t_minishell *shell, const char *command)
{
	char	*env_PATH;
	char	**splited_PATH;
	int		i;
	char	*full_command_path;

	env_PATH = get_env_value(shell, "PATH");
	if (!env_PATH)
		no_such_file_or_directory(shell, command);
	splited_PATH = ft_split(env_PATH, ":");
	free(env_PATH);
	if (!splited_PATH)
		exit(EXIT_FAILURE);
	i = 0;
	while (splited_PATH && splited_PATH[i])
	{
		//todo
		full_command_path = ft_strjoin_three(splited_PATH[i], "/", command);
		if (access(full_command_path, F_OK | X_OK) == 0)
			return (ft_array_free(splited_PATH), full_command_path);
		free(full_command_path);
		i++;
	}
	ft_array_free(splited_PATH);
	return (NULL);
}

char	*get_cmd_path(t_minishell *shell, const char *cmd_str)
{
	char	*cmd_path;

	if (ft_strchr(cmd_str, '/'))
	{
		cmd_path = ft_strdup(cmd_str);
		if (!cmd_path)
			exit(EXIT_FAILURE);
	}
	else
	{
		cmd_path = search_command_in_path(shell, cmd_str);
		if (!cmd_path)
		{
			ft_fprintf(STDERR_FILENO, "MINISHELLaaa: %s: %s\n", cmd_str,
				strerror(errno));
			exit(127);
		}
	}
	return (cmd_path);
}

void	execute_external_command(t_minishell *shell, t_command *cmd)
{
	char	*command_path;
	char	**envp_array;

	envp_array = build_envp_array(shell->env);
	command_path = get_cmd_path(shell, cmd->args[0]);
	if (!envp_array)
		exit(EXIT_FAILURE); // todo
	if (execve(command_path, cmd->args, envp_array) == -1)
	{
		// free envp
		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: %s\n", cmd->args[0],
			strerror(errno));
		// ft_fprintf(2, "%s: command not found\n", cmd->args[0]);
		free(command_path);
		exit(EXIT_FAILURE);
	}
}

void	exit_with_fd_cleanup(t_exec *exec_info)
{
	if (exec_info->input_fd != -1)
		close(exec_info->input_fd);
	if (exec_info->pipe_fds[0] != -1)
		close(exec_info->pipe_fds[0]);
	if (exec_info->pipe_fds[1] != -1)
		close(exec_info->pipe_fds[1]);
	exit(EXIT_FAILURE);
}

void	cleanup_unsed_fds(t_minishell *shell)
{
	close(shell->original_stdin);
	close(shell->original_stdout);
}

void	execute_child_process(t_minishell *shell, t_exec *exec_info,
		t_command *cmd)
{
	cleanup_unsed_fds(shell);
	if (exec_info->input_fd != STDIN_FILENO
		&& shell->commands->infile_count == 0)
	{
		if (dup2(exec_info->input_fd, STDIN_FILENO) == -1)
			exit_with_fd_cleanup(exec_info);
		close(exec_info->input_fd);
		exec_info->input_fd = -1;
	}
	if (cmd->next && shell->commands->outfile_count == 0)
	{
		if (!dup2(exec_info->pipe_fds[1], STDOUT_FILENO))
			exit_with_fd_cleanup(exec_info);
		close(exec_info->pipe_fds[0]);
		close(exec_info->pipe_fds[1]);
	}
	if (!handle_redirection(shell, cmd))
		exit(EXIT_FAILURE);
	if (exec_info->builtin_id != NOT_BUILDIN)
	{
		exec_info->builtins[exec_info->builtin_id](shell);
		exit(EXIT_SUCCESS);
	}
	else
		execute_external_command(shell, cmd);
}
