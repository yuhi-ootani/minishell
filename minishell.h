/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:16:48 by knemcova          #+#    #+#             */
/*   Updated: 2025/02/26 14:47:07 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <termios.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal;
/*signals*/
void							setup_signals(void);
void							handle_sigint(int signum);
void							setup_signals(void);
void							disable_ctrlc_display(void);
/*built-in*/
int								builtin_commands(char **args, char **envp);
void							ft_cd(char *path);
void							ft_export(char **args);
void							ft_unset(char *var);
void							ft_exit(char **args);
int								ft_isnumber(char *str);
void							ft_putendl(char *s);

/*echo_pwd_envc*/ /// done completely
int								ft_pwd(void);
int								ft_echo(char **args);
void							ft_env(char **envp);

char							*ft_strndup(const char *s, size_t n);

#endif