/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:33:48 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/03/01 16:28:41 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>    //PATH_MAX
# include <string.h>    //strcmp🚨
# include <sys/types.h> //pid_t
# include <sys/wait.h>  //waitpid
# include <unistd.h>    //getcwd
// kiki
# include <errno.h>
# include <signal.h>
# include <sys/types.h>
# include <termios.h>

extern volatile sig_atomic_t	g_signal;

// ▗▄▄▖ ▗▄▄▖  ▗▄▖ ▗▖  ▗▖▗▄▄▄▖
// ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▛▚▞▜▌  █
// ▐▛▀▘ ▐▛▀▚▖▐▌ ▐▌▐▌  ▐▌  █
// ▐▌   ▐▌ ▐▌▝▚▄▞▘▐▌  ▐▌  █

/* prototype */
char							*prompt(void);
char							*get_current_directory(void);

// ▗▖   ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖
// ▐▌   ▐▌    ▝▚▞▘ ▐▌   ▐▌ ▐▌
// ▐▌   ▐▛▀▀▘  ▐▌  ▐▛▀▀▘▐▛▀▚▖
// ▐▙▄▄▖▐▙▄▄▖▗▞▘▝▚▖▐▙▄▄▖▐▌ ▐▌

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV,
	TOKEN_EOF
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

/* prototype */
void							append_word_token(const char *input, size_t *i,
									t_token **tokens);
t_token							*create_new_token(t_token_type type,
									const char *value);
void							append_token(t_token **head, t_token *new_node);
t_token							*lexer(const char *input);
void							print_tokens(t_token *tokens);
void							free_tokens(t_token *tokens);

// ▗▄▄▖  ▗▄▖ ▗▄▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▖
// ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌   ▐▌ ▐▌
// ▐▛▀▘ ▐▛▀▜▌▐▛▀▚▖ ▝▀▚▖▐▛▀▀▘▐▛▀▚▖
// ▐▌   ▐▌ ▐▌▐▌ ▐▌▗▄▄▞▘▐▙▄▄▖▐▌ ▐▌

typedef struct s_command
{
	char						**args;
	char						*input_file;
	char						*out_file;
	int							append;
	struct s_command			*next;
}								t_command;

t_command						*parser(t_token *token_list);
// ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▖ ▗▄▄▖
// ▐▌    ▝▚▞▘ ▐▌   ▐▌   ▐▌ ▐▌  █ ▐▌ ▐▌▐▌ ▐▌
// ▐▛▀▀▘  ▐▌  ▐▛▀▀▘▐▌   ▐▌ ▐▌  █ ▐▌ ▐▌▐▛▀▚▖
// ▐▙▄▄▖▗▞▘▝▚▖▐▙▄▄▖▝▚▄▄▖▝▚▄▞▘  █ ▝▚▄▞▘▐▌ ▐▌

typedef enum e_buildin_cmd
{
	FT_ECHO,
	FT_CD,
	FT_PWD,
	FT_EXPORT,
	FT_UNSET,
	FT_ENV,
	FT_EXIT,
	FT_NOT_BUILDIN,
}								t_buildin_cmd;

// prototype
void							single_command_executor(t_command *command,
									char **envp);
t_buildin_cmd					is_builtin(char *command_str);

// Builtin functions (implement separately)
int								ft_echo(t_command *cmd);
int								ft_cd(t_command *cmd);
int								ft_pwd(t_command *cmd);
int								ft_export(t_command *cmd);
int								ft_unset(t_command *cmd);
int								ft_env(t_command *cmd);
int								ft_exit(t_command *cmd);

//                  kiki                 //

/*signals*/
void							setup_signals(void);
void							handle_sigint(int signum);
void							setup_signals(void);
void							disable_ctrlc_display(void);
// ▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖    ▗▄▄▖
// ▐▌ ▐▌  █    █  ▐▌   ▐▌
// ▐▌ ▐▌  █    █  ▐▌    ▝▀▚▖
// ▝▚▄▞▘  █  ▗▄█▄▖▐▙▄▄▖▗▄▄▞▘

int								ft_isnumber(char *str);
void							ft_putendl(char *s);

#endif
