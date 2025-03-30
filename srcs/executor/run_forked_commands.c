
#include "../../include/minishell.h"

static void	wait_for_all_children(pid_t *pids, int count, t_minishell *shell)
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

static void	close_and_update_input(t_exec *exec_info, t_command *cmd)
{
	if (exec_info->input_fd != STDIN_FILENO)
		close(exec_info->input_fd);
	if (cmd->next)
	{
		close(exec_info->pipe_fds[1]);
		exec_info->input_fd = exec_info->pipe_fds[0];
	}
}

// // WEFEXITED turns true if the code was ended by exit return
// // WEXITSTATUS holds the number exit give
// // WIFSIGNALED turns true if the code was ended by a signl
// // WTERMSIG holds the number of exit of the singal

static void	create_child_process(t_minishell *shell, t_exec *exec_info,
		t_command *cmd, pid_t *pid)
{
	*pid = fork();
	if (*pid == 0)
	{
		setup_signals_child();
		execute_child_process(shell, exec_info, cmd);
	}
	else if (*pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE); // todo
	}
}

bool	setup_pipe_if_needed(t_minishell *shell, t_exec *exec_info,
		t_command *cmd)
{
	if (cmd->next)
	{
		if (pipe(exec_info->pipe_fds) < 0)
		{
			shell->exit_status = EXIT_FAILURE;
			return (false);
		}
	}
	return (true);
}

void	run_forked_commands(t_minishell *shell, t_exec *exec_info)
{
	int					i;
	pid_t				pids[1000];
	t_command			*current;
	struct sigaction	previous_int;

	i = 0;
	current = shell->commands;
	while (current)
	{
		if (!setup_pipe_if_needed(shell, exec_info, current))
			return ;
		create_child_process(shell, exec_info, current, &pids[i]);
		close_and_update_input(exec_info, current);
		current = current->next;
		i++;
	}
	sigaction(SIGINT, NULL, &previous_int);
	signal(SIGINT, SIG_IGN);
	wait_for_all_children(pids, i, shell);
	sigaction(SIGINT, &previous_int, NULL);
}
