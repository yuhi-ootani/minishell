

#include "../../include/minishell.h"

t_env	*env_duplication(char **envp_srcs)
{
	t_env	*new_env;
	t_env	*head_of_copied_env;
	size_t	i;
	char	**splited_env;

	i = 0;
	head_of_copied_env = NULL;
	while (envp_srcs[i])
	{
		splited_env = ft_split(envp_srcs[i], "=");
		if (!splited_env || !splited_env[0])
		{
			return (free_copied_env(head_of_copied_env), NULL);
		}
		new_env = create_new_env_util(splited_env[0], splited_env[1], NULL);
		if (!new_env)
		{
			return (free_copied_env(head_of_copied_env), NULL);
		}
		env_add_back_util(&head_of_copied_env, new_env);
		ft_array_free(splited_env);
		i++;
	}
	return (head_of_copied_env);
}

void	keep_original_fds(t_minishell *shell)
{
	shell->original_stdin = dup2(STDIN_FILENO, 3);
	if (shell->original_stdin == -1)
		exit(FAIL_DUP);
	shell->original_stdout = dup2(STDOUT_FILENO, 4);
	if (shell->original_stdout == -1)
		exit(FAIL_DUP);
}

void	init_shell_struct(t_minishell *shell, char **envp)
{
	shell->env = NULL;
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->exit_status = 0;
	keep_original_fds(shell);
	shell->env = env_duplication(envp);
	if (!shell->env)
	{
		close(shell->original_stdin);
		close(shell->original_stdout);
		exit(EXIT_FAILURE);
	}
}
