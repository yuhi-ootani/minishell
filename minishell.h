#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

/*built-in*/
void ft_echo(char **args);
void ft_cd(char *path);
void ft_pwd();
void ft_export(char **args);
void ft_unset(char *var);
void ft_env(char **envp);
void ft_exit();

#endif