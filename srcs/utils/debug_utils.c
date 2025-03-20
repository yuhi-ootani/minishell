
#include "../../include/minishell.h"

// ▗▄▄▖ ▗▄▄▖ ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖     ▗▄▄▖ ▗▄▖ ▗▖  ▗▖▗▖  ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄
// ▐▌ ▐▌▐▌ ▐▌  █  ▐▛▚▖▐▌  █      ▐▌   ▐▌ ▐▌▐▛▚▞▜▌▐▛▚▞▜▌▐▌ ▐▌▐▛▚▖▐▌▐▌  █
// ▐▛▀▘ ▐▛▀▚▖  █  ▐▌ ▝▜▌  █      ▐▌   ▐▌ ▐▌▐▌  ▐▌▐▌  ▐▌▐▛▀▜▌▐▌ ▝▜▌▐▌  █
// ▐▌   ▐▌ ▐▌▗▄█▄▖▐▌  ▐▌  █      ▝▚▄▄▖▝▚▄▞▘▐▌  ▐▌▐▌  ▐▌▐▌ ▐▌▐▌  ▐▌▐▙▄▄▀

void	print_command(t_command *command, int cmd_index)
{
	size_t	i;

	i = 0;
	printf("Command %d:\n", cmd_index);
	// Print arguments
	if (command->args)
	{
		while (command->args[i])
		{
			printf("  Arg %ld: %s\n", i, command->args[i]);
			i++;
		}
	}
	else
	{
		printf("  No arguments\n");
	}
	// Print redirections, if any
	if (command->input_file)
		printf("  Input redirection: %s\n", command->input_file);
	if (command->heredoc_count > 0)
	{
		i = 0;
		while (i < command->heredoc_count)
		{
			printf("heredoc_files[%ld]:%s\n", i, command->heredoc_files[i]);
			i++;
		}
	}
	if (command->out_file)
	{
		printf("  Output redirection: %s\n", command->out_file);
		printf("  Append mode: %s\n", command->is_append ? "Yes" : "No");
	}
	printf("\n");
}

// Print all commands in the linked list
void	print_commands(t_command *head)
{
	int cmd_index;
	t_command *current;

	cmd_index = 0;
	current = head;
	while (current)
	{
		print_command(current, cmd_index);
		current = current->next;
		cmd_index++;
	}
}


