/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:13 by oyuhi             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/03/14 18:41:57 by oyuhi            ###   ########.fr       */
=======
/*   Updated: 2025/03/15 17:50:38 by oyuhi            ###   ########.fr       */
>>>>>>> yuhi
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*search_command_in_path(const char *command)
{
	char	*path_env;
	char	**splited_paths;
	int		i;
	char	*tmp;
	char	*full_command_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	splited_paths = ft_split(path_env, ':');
	i = 0;
	while (splited_paths && splited_paths[i])
	{
		tmp = ft_strjoin(splited_paths[i], "/");
		full_command_path = ft_strjoin(tmp, command);
		free(tmp);
		if (access(full_command_path, F_OK | X_OK) == 0)
			return (ft_array_free(splited_paths), full_command_path);
		free(full_command_path);
		i++;
	}
	ft_array_free(splited_paths);
	return (NULL);
}

void	execute_external_command(t_minishell *shell)
{
	char	*command_path;
	char	**envp_array;

	if (strchr(shell->commands->args[0], '/') == NULL)
	// I think it needs modified
	{
		command_path = search_command_in_path(shell->commands->args[0]);
		if (!command_path)
		{
			fprintf(stderr, "%s: command not found\n",
				shell->commands->args[0]);
			// modified
			exit(EXIT_FAILURE);
			// modified
		}
	}
	else
		command_path = strdup(shell->commands->args[0]);
	envp_array = build_envp_array(shell->env);
	if (!envp_array)
		exit(EXIT_FAILURE); // todo
	// printf("%s\n", envp[2]);
	if (execve(command_path, shell->commands->args, envp_array) == -1)
	{
		// free envp
		fprintf(stderr, "%s: command not found\n", shell->commands->args[0]);
		free(command_path);
		exit(EXIT_FAILURE);
	}
}

t_builtin_id	is_builtin(char *command_str)
{
	if (strcmp(command_str, "echo") == 0)
		return (FT_ECHO);
	else if (strcmp(command_str, "cd") == 0)
		return (FT_CD);
	else if (strcmp(command_str, "pwd") == 0)
		return (FT_PWD);
	else if (strcmp(command_str, "export") == 0)
		return (FT_EXPORT);
	else if (strcmp(command_str, "unset") == 0)
		return (FT_UNSET);
	else if (strcmp(command_str, "env") == 0)
		return (FT_ENV);
	else if (strcmp(command_str, "exit") == 0)
		return (FT_EXIT);
	else
		return (NOT_BUILDIN);
}

static void	execute_child_process(t_minishell *shell, t_exec *exec_info)
{
	if (shell->commands->is_heredoc == false
		&& exec_info->input_fd != STDIN_FILENO)
	{
		dup2(exec_info->input_fd, STDIN_FILENO);
		close(exec_info->input_fd);
	}
	if (shell->commands->next)
	{
		dup2(exec_info->pipe_fds[1], STDOUT_FILENO);
		close(exec_info->pipe_fds[0]);
		close(exec_info->pipe_fds[1]);
	}
	handle_redirection(shell->commands);
	if (exec_info->builtin_id != NOT_BUILDIN)
	{
		exec_info->builtins[exec_info->builtin_id](shell);
		exit(EXIT_SUCCESS);
	}
	else
		execute_external_command(shell);
}

static void	run_single_builtin_in_parent(t_minishell *shell, t_exec *exec_info)
{
	handle_redirection(shell->commands);
	exec_info->builtins[exec_info->builtin_id](shell);
}

bool	is_single_builtin_command(t_minishell *shell, t_exec *exec_info)
{
	return (shell->commands->next == NULL
		&& exec_info->builtin_id != NOT_BUILDIN);
}

void	run_forked_commands(t_minishell *shell, t_exec *exec_info)
{
	int	i;

	pid_t pids[1000]; // Array to store PIDs for all commands
	i = 0;
	while (shell->commands)
	{
		if (shell->commands->next)
		{
			if (pipe(exec_info->pipe_fds) < 0)
			{
				perror("pipe");
				return ;
			}
		}
		pids[i] = fork();
		if (pids[i] == 0)
			execute_child_process(shell, exec_info);
		else if (pids[i] < 0)
		{
			perror("fork");     // modified
			exit(EXIT_FAILURE); // modified
		}
		if (!shell->commands->next)
			waitpid(pids[i], shell->exit_status, 0);
		if (exec_info->input_fd != STDIN_FILENO)
			close(exec_info->input_fd);
		if (shell->commands->next)
		{
			close(exec_info->pipe_fds[1]);
			exec_info->input_fd = exec_info->pipe_fds[0];
		}
		shell->commands = shell->commands->next;
		i++;
	}
	// Now wait for all children
	for (int j = 0; j < i; j++)
		waitpid(pids[j], shell->exit_status, 0);
}

static void	init_exec_info(t_exec *exec_info)
{
	exec_info->input_fd = STDIN_FILENO;
	exec_info->builtin_id = NOT_BUILDIN;
	exec_info->builtins[FT_ECHO] = ft_echo;
	exec_info->builtins[FT_CD] = ft_cd;
	exec_info->builtins[FT_PWD] = ft_pwd;
	exec_info->builtins[FT_EXPORT] = ft_export;
	exec_info->builtins[FT_UNSET] = ft_unset;
	exec_info->builtins[FT_ENV] = ft_env;
	exec_info->builtins[FT_EXIT] = ft_exit;
}

void	command_executor(t_minishell *shell)
{
	t_exec	exec_info;

	init_exec_info(&exec_info);
	exec_info.builtin_id = is_builtin(shell->commands->args[0]);
	if (is_single_builtin_command(shell, &exec_info))
	{
		run_single_builtin_in_parent(shell, &exec_info);
	}
	else
		run_forked_commands(shell, &exec_info);
}

// #define MAX_COMMANDS 1024

// void	single_command_executor(t_command *command, char **envp)
// {
// 	pid_t pids[MAX_COMMANDS];
// 	size_t i = 0;
// 	static int (*builtin_funcs[])(t_command *) = {ft_echo, ft_cd, ft_pwd,
// 		ft_export, ft_unset, ft_env, ft_exit};
// 	int pipefd[2];
// 	int in_fd = STDIN_FILENO;
// 	// pid_t pid;
// 	int status;

// 	while (command)
// 	{
// 		if (command->next)
// 		{
// 			if (pipe(pipefd) < 0)
// 			{
// 				perror("pipe");
// 				return ;
// 			}
// 		}
// 		pids[i] = fork();
// 		if (pids[i] == 0)
// 		{
// 			if (in_fd != STDIN_FILENO)
// 			{
// 				dup2(in_fd, STDIN_FILENO);
// 				close(in_fd);
// 			}
// 			if (command->next)
// 			{
// 				dup2(pipefd[1], STDOUT_FILENO);
// 				close(pipefd[0]);
// 				close(pipefd[1]);
// 			}
// 			handle_redirection(command);
// 			t_builtin_id buildin_index = is_builtin(command->args[0]);

// 			if (buildin_index != NOT_BUILDIN)
// 				builtin_funcs[buildin_index](command); // Execute the function
// 			else
// 				execute_external_command(command, envp);
// 		}
// 		else if (pids[i] < 0)
// 		{
// 			perror("fork");     // modified
// 			exit(EXIT_FAILURE); // modified
// 		}

// 		// waitpid(pid, &status, 0);

// 		if (in_fd != STDIN_FILENO)
// 			close(in_fd);

// 		if (command->next)
// 		{
// 			close(pipefd[1]);
// 			in_fd = pipefd[0];
// 		}
// 		command = command->next;
// 		i++;
// 	}

// 	// Wait for all children after forking them all.
// 	for (size_t j = 0; j < i; j++)
// 		waitpid(pids[j], &status, 0);
// }