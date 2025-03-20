/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:33:48 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/03/20 16:28:15 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h> //O_WRONLY O_APPEND O_CREAT
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>    //PATH_MAX
# include <string.h>    //strcmp🚨
# include <sys/types.h> //pid_t
# include <sys/wait.h>  //waitpid
# include <termios.h>
# include <unistd.h> //getcwd

# define delimiters " \t\n"

typedef struct s_minishell		t_minishell;

// ▗▖  ▗▖ ▗▄▖ ▗▄▄▄▖▗▖  ▗▖
// ▐▛▚▞▜▌▐▌ ▐▌  █  ▐▛▚▖▐▌
// ▐▌  ▐▌▐▛▀▜▌  █  ▐▌ ▝▜▌
// ▐▌  ▐▌▐▌ ▐▌▗▄█▄▖▐▌  ▐▌

typedef struct s_env
{
	char						*name;
	char						*value;
	struct s_env				*next;
}								t_env;

/* prototype */
void							init_shell_struct(t_minishell *shell,
									char **envp);

// RubiFont
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
	TOKEN_EOF
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

/* prototype */
void							add_word_token(const char *input, size_t *i,
									t_token **tokens);
t_token							*create_new_token(t_token_type type,
									const char *value);
void							add_token(t_token **head, t_token *new_node);
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
	bool						is_heredoc;
	char						**heredoc_files;
	size_t						heredoc_count;
	char						*out_file;
	bool						is_append;
	struct s_command			*next;
}								t_command;

t_command						*parser(t_token *token_list);

// ▗▄▄▖ ▗▄▄▄▖▗▄▄▄ ▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▄▄▄▖ ▗▄▖ ▗▖  ▗▖
// ▐▌ ▐▌▐▌   ▐▌  █  █  ▐▌ ▐▌▐▌   ▐▌     █    █  ▐▌ ▐▌▐▛▚▖▐▌
// ▐▛▀▚▖▐▛▀▀▘▐▌  █  █  ▐▛▀▚▖▐▛▀▀▘▐▌     █    █  ▐▌ ▐▌▐▌ ▝▜▌
// ▐▌ ▐▌▐▙▄▄▖▐▙▄▄▀▗▄█▄▖▐▌ ▐▌▐▙▄▄▖▝▚▄▄▖  █  ▗▄█▄▖▝▚▄▞▘▐▌  ▐▌

void							handle_redirection(t_command *command);

//  ▗▄▄▖▗▄▄▄▖ ▗▄▄▖▗▖  ▗▖ ▗▄▖ ▗▖
// ▐▌     █  ▐▌   ▐▛▚▖▐▌▐▌ ▐▌▐▌
//  ▝▀▚▖  █  ▐▌▝▜▌▐▌ ▝▜▌▐▛▀▜▌▐▌
// ▗▄▄▞▘▗▄█▄▖▝▚▄▞▘▐▌  ▐▌▐▌ ▐▌▐▙▄▄▖

extern volatile sig_atomic_t	g_signal;

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
size_t							count_env_util(t_env *env);
t_env							*create_new_env_util(const char *new_name,
									const char *new_value, t_env *new_next);
void							env_add_back_util(t_env **copied_env,
									t_env *new_env);
void							free_copied_env(t_env *env);

// ▗▄▄▄▖      ▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▖   ▗▖
//   █        ▐▛▚▞▜▌  █  ▐▛▚▖▐▌  █  ▐▌   ▐▌ ▐▌▐▌   ▐▌   ▐▌
//   █        ▐▌  ▐▌  █  ▐▌ ▝▜▌  █   ▝▀▚▖▐▛▀▜▌▐▛▀▀▘▐▌   ▐▌
//   █  ▗▄▄▄▄ ▐▌  ▐▌▗▄█▄▖▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖▐▙▄▄▖

typedef struct s_minishell
{
	t_env						*env;
	t_token						*tokens;
	t_command					*commands;
	int							*exit_status;
	int							original_stdin;
	int							original_stdout;
}								t_minishell;

// ▗▄▄▄▖▗▖  ▗▖▗▄▄▖  ▗▄▖ ▗▖  ▗▖▗▄▄▄
// ▐▌    ▝▚▞▘ ▐▌ ▐▌▐▌ ▐▌▐▛▚▖▐▌▐▌  █
// ▐▛▀▀▘  ▐▌  ▐▛▀▘ ▐▛▀▜▌▐▌ ▝▜▌▐▌  █
// ▐▙▄▄▖▗▞▘▝▚▖▐▌   ▐▌ ▐▌▐▌  ▐▌▐▙▄▄▀

typedef struct s_expanded_str
{
	char						*buffer;
	size_t						size;
	size_t						index;
	bool						in_single_quote;
	bool						in_double_quote;
}								t_expanded_str;

void							expand_commands(t_minishell *shell);
char							*get_expanded_str(t_minishell *shell,
									const char *src_input);
char							**expander(t_minishell *shell, char **args);

// ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▖ ▗▄▄▖
// ▐▌    ▝▚▞▘ ▐▌   ▐▌   ▐▌ ▐▌  █ ▐▌ ▐▌▐▌ ▐▌
// ▐▛▀▀▘  ▐▌  ▐▛▀▀▘▐▌   ▐▌ ▐▌  █ ▐▌ ▐▌▐▛▀▚▖
// ▐▙▄▄▖▗▞▘▝▚▖▐▙▄▄▖▝▚▄▄▖▝▚▄▞▘  █ ▝▚▄▞▘▐▌ ▐▌

typedef enum e_buildin_index
{
	FT_ECHO,
	FT_CD,
	FT_PWD,
	FT_EXPORT,
	FT_UNSET,
	FT_ENV,
	FT_EXIT,
	NOT_BUILDIN,
}								t_builtin_id;

# define NUM_BUILTINS 7
typedef struct s_exec
{
	int							input_fd;
	int							pipe_fds[2];
	t_builtin_id				builtin_id;
	void						(*builtins[NUM_BUILTINS])(t_minishell *);
}								t_exec;

// prototype
void							command_executor(t_minishell *shell);
char							**build_envp_array(t_env *env);

// ▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖ ▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖ ▗▄▄▖
// ▐▌ ▐▌▐▌ ▐▌  █  ▐▌   █    █  ▐▛▚▖▐▌▐▌
// ▐▛▀▚▖▐▌ ▐▌  █  ▐▌   █    █  ▐▌ ▝▜▌ ▝▀▚▖
// ▐▙▄▞▘▝▚▄▞▘▗▄█▄▖▐▙▄▄▖█  ▗▄█▄▖▐▌  ▐▌▗▄▄▞▘

// Builtin functions (implement separately)
void							ft_echo(t_minishell *shell);
void							ft_cd(t_minishell *shell);
void							ft_pwd(t_minishell *shell);
void							ft_export(t_minishell *shell);
void							sort_and_print_env(t_env **copied_env);
void							ft_unset(t_minishell *shell);
void							ft_env(t_minishell *shell);
void							ft_exit(t_minishell *shell);

typedef enum e_exit_status
{
	FAIL_DUP = 255,
}								t_exit_status;

// ▗▄▄▄ ▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖ ▗▄▄▖    ▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖    ▗▄▄▖
// ▐▌  █▐▌   ▐▌ ▐▌▐▌ ▐▌▐▌       ▐▌ ▐▌  █    █  ▐▌   ▐▌
// ▐▌  █▐▛▀▀▘▐▛▀▚▖▐▌ ▐▌▐▌▝▜▌    ▐▌ ▐▌  █    █  ▐▌    ▝▀▚▖
// ▐▙▄▄▀▐▙▄▄▖▐▙▄▞▘▝▚▄▞▘▝▚▄▞▘    ▝▚▄▞▘  █  ▗▄█▄▖▐▙▄▄▖▗▄▄▞▘

void							print_commands(t_command *head);

#endif
