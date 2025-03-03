/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:13 by oyuhi             #+#    #+#             */
/*   Updated: 2025/03/02 13:03:51 by oyuhi            ###   ########.fr       */
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

void	execute_external_command(t_command *command, char **envp)
{
	char	*command_path;

	if (strchr(command->args[0], '/') == NULL) // I think it needs modified
	{
		command_path = search_command_in_path(command->args[0]);
		if (!command_path)
		{
			fprintf(stderr, "%s: command not found\n", command->args[0]);
			// modified
			exit(EXIT_FAILURE);
			// modified
		}
	}
	else
		command_path = strdup(command->args[0]);
	if (execve(command_path, command->args, envp) == -1)
	{
		fprintf(stderr, "%s: command not found\n", command->args[0]);
		free(command_path);
		exit(EXIT_FAILURE);
	}
}

t_buildin_cmd	is_builtin(char *command_str)
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
		return (FT_NOT_BUILDIN);
}

static void	execute_child_process(t_command *command, int input_fd, int *pipefd,
		char **envp)
{
	t_buildin_cmd	buildin_index;

	static int (*builtin_funcs[])(t_command *) = {ft_echo, ft_cd, ft_pwd,
		ft_export, ft_unset, ft_env, ft_exit};
	if (command->is_heredoc == false && input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	handle_redirection(command);
	if (command->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	buildin_index = is_builtin(command->args[0]);
	if (buildin_index != FT_NOT_BUILDIN)
		builtin_funcs[buildin_index](command); // Execute the function
	else
		execute_external_command(command, envp);
}

void	command_executor(t_command *command, char **envp)
{
	int		pipefd[2];
	int		input_fd;
	pid_t	pid;
	int		status;

	input_fd = STDIN_FILENO;
	while (command)
	{
		if (command->next)
		{
			if (pipe(pipefd) < 0)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		if (pid == 0)
			execute_child_process(command, input_fd, pipefd, envp);
		else if (pid < 0)
		{
			perror("fork");     // modified
			exit(EXIT_FAILURE); // modified
		}
		waitpid(pid, &status, 0);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		if (command->next)
		{
			close(pipefd[1]);
			input_fd = pipefd[0];
		}
		command = command->next;
	}
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
// 			t_buildin_cmd buildin_index = is_builtin(command->args[0]);

// 			if (buildin_index != FT_NOT_BUILDIN)
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