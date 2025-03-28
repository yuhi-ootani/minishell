/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:13 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/28 16:04:45 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*search_command_in_path(const char *cmd)
{
	char	*path_env;
	char	**splited_paths;
	int		i;
	char	*tmp;
	char	*full_command_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	splited_paths = ft_split(path_env, ":");
	i = 0;
	while (splited_paths && splited_paths[i])
	{
		tmp = ft_strjoin(splited_paths[i], "/");
		full_command_path = ft_strjoin(tmp, cmd);
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

	if (ft_strchr(shell->commands->args[0], '/') == NULL)
	{
		command_path = search_command_in_path(shell->commands->args[0]);
		if (!command_path)
		{
			ft_fprintf(2, "%s: command not found\n", shell->commands->args[0]);
			(shell->exit_status) = 127;
			exit(127);
		}
	}
	else
		command_path = ft_strdup(shell->commands->args[0]);
	envp_array = build_envp_array(shell->env);
	if (!envp_array)
		exit(EXIT_FAILURE); // todo
	// printf("%s\n", envp[2]);
	if (execve(command_path, shell->commands->args, envp_array) == -1)
	{
		// free envp
		ft_fprintf(2, "%s: command not found\n", shell->commands->args[0]);
		free(command_path);
		exit(EXIT_FAILURE);
	}
}

t_builtin_id	is_builtin(char *command_str)
{
	if (ft_strcmp(command_str, "echo") == 0)
		return (FT_ECHO);
	else if (ft_strcmp(command_str, "cd") == 0)
		return (FT_CD);
	else if (ft_strcmp(command_str, "pwd") == 0)
		return (FT_PWD);
	else if (ft_strcmp(command_str, "export") == 0)
		return (FT_EXPORT);
	else if (ft_strcmp(command_str, "unset") == 0)
		return (FT_UNSET);
	else if (ft_strcmp(command_str, "env") == 0)
		return (FT_ENV);
	else if (ft_strcmp(command_str, "exit") == 0)
		return (FT_EXIT);
	else
		return (NOT_BUILDIN);
}

static void	execute_child_process(t_minishell *shell, t_exec *exec_info)
{
	// 	size_t	infile_count;
	// 	infile_count = shell->commands->infile_count;
	// if (infile_count > 0 && shell->commands->infiles[infile_count
	// 	- 1].type == TOKEN_HEREDOC && exec_info->input_fd != STDIN_FILENO)
	if (exec_info->input_fd != STDIN_FILENO
		&& shell->commands->infile_count == 0)
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
	if (exec_info->builtin_id == FT_EXIT)
		exit(shell->exit_status);
}

bool	is_single_builtin_command(t_minishell *shell, t_exec *exec_info)
{
	return (shell->commands->next == NULL
		&& exec_info->builtin_id != NOT_BUILDIN);
}

static void	backup_and_ignore_sigint(struct sigaction *old_int)
{
	sigaction(SIGINT, NULL, old_int);
	signal(SIGINT, SIG_IGN);
}

static void	restore_sigint(struct sigaction *old_int)
{
	sigaction(SIGINT, old_int, NULL);
}

void	handle_pipe_and_fork(t_minishell *shell, t_exec *exec_info, pid_t *pids,
		int i)
{
	if (shell->commands->next)
	{
		if (pipe(exec_info->pipe_fds) < 0)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
	pids[i] = fork();
	if (pids[i] == 0)
	{
		setup_signals_child();
		execute_child_process(shell, exec_info);
	}
	else if (pids[i] < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	update_input_fd(t_minishell *shell, t_exec *exec_info)
{
	if (exec_info->input_fd != STDIN_FILENO)
		close(exec_info->input_fd);
	if (shell->commands->next)
	{
		close(exec_info->pipe_fds[1]);
		exec_info->input_fd = exec_info->pipe_fds[0];
	}
}

void	wait_for_children(pid_t *pids, int count, t_minishell *shell)
{
	int	j;

	j = 0;
	while (j < count)
	{
		waitpid(pids[j], &shell->exit_status, 0);
		if (WIFEXITED(shell->exit_status))
			shell->exit_status = WEXITSTATUS(shell->exit_status);
		else if (WIFSIGNALED(shell->exit_status))
		{
			printf("\n");
			shell->exit_status = 128 + WTERMSIG(shell->exit_status);
		}
		else
			shell->exit_status = 1;
		j++;
	}
}
// WEFEXITED turns true if the code was ended by exit return
// WEXITSTATUS holds the number exit give
// WIFSIGNALED turns true if the code was ended by a signl
// WTERMSIG holds the number of exit of the singal

void	run_forked_commands(t_minishell *shell, t_exec *exec_info)
{
	int					i;
	struct sigaction	old_int;
	pid_t				pids[1000];

	i = 0;
	while (shell->commands)
	{
		handle_pipe_and_fork(shell, exec_info, pids, i);
		update_input_fd(shell, exec_info);
		shell->commands = shell->commands->next;
		i++;
	}
	backup_and_ignore_sigint(&old_int);
	wait_for_children(pids, i, shell);
	restore_sigint(&old_int);
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

	if (!shell->commands)
		return ;
	init_exec_info(&exec_info);
	if (!shell->commands->args)
	{
		handle_redirection(shell->commands);
		return ;
	}
	exec_info.builtin_id = is_builtin(shell->commands->args[0]);
	if (is_single_builtin_command(shell, &exec_info))
	{
		run_single_builtin_in_parent(shell, &exec_info);
	}
	else
	{
		run_forked_commands(shell, &exec_info);
	}
}

// #define MAX_COMMANDS 1024

// void	single_command_executor(t_command *cmd, char **envp)
// {
// 	pid_t			pids[MAX_COMMANDS];
// 	size_t			i;
// 	int				pipefd[2];
// 	int				in_fd;
// 	int				status;
// 	t_builtin_id	buildin_index;

// 	i = 0;
// 	static int (*builtin_funcs[])(t_command *) = {ft_echo, ft_cd, ft_pwd,
// 		ft_export, ft_unset, ft_env, ft_exit};
// 	in_fd = STDIN_FILENO;
// 	// pid_t pid;
// 	while (cmd)
// 	{
// 		if (cmd->next)
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
// 			if (cmd->next)
// 			{
// 				dup2(pipefd[1], STDOUT_FILENO);
// 				close(pipefd[0]);
// 				close(pipefd[1]);
// 			}
// 			handle_redirection(cmd);
// 			buildin_index = is_builtin(cmd->args[0]);
// 			if (buildin_index != NOT_BUILDIN)
// 				builtin_funcs[buildin_index](cmd); // Execute the function
// 			else
// 				execute_external_command(cmd, envp);
// 		}
// 		else if (pids[i] < 0)
// 		{
// 			perror("fork");     // modified
// 			exit(EXIT_FAILURE); // modified
// 		}
// 		// waitpid(pid, &status, 0);
// 		if (in_fd != STDIN_FILENO)
// 			close(in_fd);
// 		if (cmd->next)
// 		{
// 			close(pipefd[1]);
// 			in_fd = pipefd[0];
// 		}
// 		cmd = cmd->next;
// 		i++;
// 	}
// 	// Wait for all children after forking them all.
// 	for (size_t j = 0; j < i; j++)
// 		waitpid(pids[j], &status, 0);
// }

// /*yuyu code with my signals, long version */

// void	run_forked_commands(t_minishell *shell, t_exec *exec_info)
// {
// 	int					i;
// 	struct sigaction	old_int;

// 	pid_t pids[1000]; // Array to store PIDs for all commands
// 	i = 0;
// 	while (shell->commands)
// 	{
// 		if (shell->commands->next)
// 		{
// 			if (pipe(exec_info->pipe_fds) < 0)
// 			{
// 				perror("pipe");
// 				return ;
// 			}
// 		}
// 		pids[i] = fork();
// 		if (pids[i] == 0)
// 		{
// 			setup_signals_child();
// 			execute_child_process(shell, exec_info);
// 		}
// 		else if (pids[i] < 0)
// 		{
// 			perror("fork");     // modified
// 			exit(EXIT_FAILURE); // modified
// 		}
// 		if (exec_info->input_fd != STDIN_FILENO)
// 			close(exec_info->input_fd);
// 		if (shell->commands->next)
// 		{
// 			close(exec_info->pipe_fds[1]);
// 			exec_info->input_fd = exec_info->pipe_fds[0];
// 		}
// 		shell->commands = shell->commands->next;
// 		i++;
// 	}
// 	sigaction(SIGINT, NULL, &old_int);
// 	signal(SIGINT, SIG_IGN);
// 	for (int j = 0; j < i; j++)
// 	{
// 		waitpid(pids[j], &shell->exit_status, 0);
// 		if (WIFEXITED(shell->exit_status))
// 			shell->exit_status = WEXITSTATUS(shell->exit_status);
// 		else if (WIFSIGNALED(shell->exit_status))
// 		{
// 			printf("\n");
// 			shell->exit_status = 128 + WTERMSIG(shell->exit_status);
// 		}
// 		else
// 			shell->exit_status = 1;
// 	}
// 	sigaction(SIGINT, &old_int, NULL);
// }