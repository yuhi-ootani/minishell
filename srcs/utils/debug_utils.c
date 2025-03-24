
#include "../../include/minishell.h"

// ▗▄▄▖ ▗▄▄▖ ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖     ▗▄▄▖ ▗▄▖ ▗▖  ▗▖▗▖  ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄
// ▐▌ ▐▌▐▌ ▐▌  █  ▐▛▚▖▐▌  █      ▐▌   ▐▌ ▐▌▐▛▚▞▜▌▐▛▚▞▜▌▐▌ ▐▌▐▛▚▖▐▌▐▌  █
// ▐▛▀▘ ▐▛▀▚▖  █  ▐▌ ▝▜▌  █      ▐▌   ▐▌ ▐▌▐▌  ▐▌▐▌  ▐▌▐▛▀▜▌▐▌ ▝▜▌▐▌  █
// ▐▌   ▐▌ ▐▌▗▄█▄▖▐▌  ▐▌  █      ▝▚▄▄▖▝▚▄▞▘▐▌  ▐▌▐▌  ▐▌▐▌ ▐▌▐▌  ▐▌▐▙▄▄▀

void	print_command(t_command *cmd, int cmd_index)
{
	size_t	i;

	i = 0;
	printf("cmd %d:\n", cmd_index);
	// Print arguments
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			printf("  Arg %ld: %s\n", i, cmd->args[i]);
			i++;
		}
	}
	else
	{
		printf("  No arguments\n");
	}
	// Print redirections, if any
	if (cmd->input_file)
		printf("  Input redirection: %s\n", cmd->input_file);
	if (cmd->heredoc_count > 0)
	{
		i = 0;
		while (i < cmd->heredoc_count)
		{
			printf("heredoc_files[%ld]:%s\n", i, cmd->heredoc_files[i]);
			i++;
		}
	}
	if (cmd->out_file)
	{
		printf("  Output redirection: %s\n", cmd->out_file);
		printf("  Append mode: %s\n", cmd->is_append ? "Yes" : "No");
	}
	printf("\n");
}

// Print all commands in the linked list
void	print_commands(t_command *head)
{
	int			cmd_index;
	t_command	*current;

	cmd_index = 0;
	current = head;
	while (current)
	{
		print_command(current, cmd_index);
		current = current->next;
		cmd_index++;
	}
}

// ▗▄▄▄▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌▗▞▘▐▌   ▐▛▚▖▐▌  █     ▗▞▘▐▌   ▐▌ ▐▌
//   █ ▐▌ ▐▌▐▛▚▖ ▐▛▀▀▘▐▌ ▝▜▌  █   ▗▞▘  ▐▛▀▀▘▐▛▀▚▖
//   █ ▝▚▄▞▘▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌▗▄█▄▖▐▙▄▄▄▖▐▙▄▄▖▐▌ ▐▌

void	print_tokens(t_token *tokens)
{
	const char	*type_str;

	while (tokens)
	{
		switch (tokens->type)
		{
		case TOKEN_WORD:
			type_str = "ARG";
			break ;
		case TOKEN_PIPE:
			type_str = "PIPE";
			break ;
		case TOKEN_REDIR_IN:
			type_str = "REDIR_IN";
			break ;
		case TOKEN_REDIR_OUT:
			type_str = "REDIR_OUT";
			break ;
		case TOKEN_APPEND:
			type_str = "REDIR_APPEND";
			break ;
		case TOKEN_HEREDOC:
			type_str = "HEREDOC";
			break ;
		case TOKEN_EOF:
			type_str = "EOF";
			break ;
		default:
			type_str = "UNKNOWN";
			break ;
		}
		printf("[%s] '%s'\n", type_str, tokens->value ? tokens->value : "");
		tokens = tokens->next;
	}
}
