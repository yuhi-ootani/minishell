/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:13 by oyuhi             #+#    #+#             */
/*   Updated: 2025/02/27 15:16:33 by oyuhi            ###   ########.fr       */
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

int	execute_external_command(t_command *command, char **envp)
{
	pid_t	pid;
	int		status;
	char	*command_path;

	pid = fork();
	if (pid == 0)
	{
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
	else if (pid < 0)
	{
		perror("fork"); // modified
		return (-1);    // modified
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (-1);
	}
	return (status);
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

void	single_command_executor(t_command *command, char **envp)
{
	static int (*builtin_funcs[])(t_command *) = {ft_echo, ft_cd, ft_pwd,
		ft_export, ft_unset, ft_env, ft_exit};

	t_buildin_cmd buildin_cmd_nbr = is_builtin(command->args[0]);
	if (buildin_cmd_nbr != FT_NOT_BUILDIN)
		builtin_funcs[buildin_cmd_nbr](command); // Execute the function
	else
		execute_external_command(command, envp);
}