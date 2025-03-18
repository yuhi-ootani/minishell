/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:17:05 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/18 19:23:02 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_putendl(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i] != '\0')
	{
		write(1, &s[i], 1);
		i++;
	}
	write(1, "\n", 1);
}

// ▗▄▄▄▖▗▖  ▗▖▗▖  ▗▖
// ▐▌   ▐▛▚▖▐▌▐▌  ▐▌
// ▐▛▀▀▘▐▌ ▝▜▌▐▌  ▐▌
// ▐▙▄▄▖▐▌  ▐▌ ▝▚▞▘

size_t	count_env_util(t_env *env)
{
	size_t	count;

	count = 0;
	if (!env)
		return (count);
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

t_env	*create_new_env_util(const char *new_name, const char *new_value,
		t_env *new_next)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->name = ft_strdup(new_name);
	if (!new_env->name)
	{
		free(new_env);
		return (NULL);
	}
	if (new_value)
	{
		new_env->value = ft_strdup(new_value);
		if (!new_env->value)
		{
			free(new_env->name);
			free(new_env);
			return (NULL);
		}
	}
	else
		new_env->value = NULL;
	new_env->next = new_next;
	return (new_env);
}

void	env_add_back_util(t_env **copied_env, t_env *new_env)
{
	t_env	*tmp;

	tmp = *copied_env;
	if (!tmp)
	{
		*copied_env = new_env;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env;
}

int	ft_fputchar_fd(int fd, char c)
{
	return (write(fd, &c, 1));
}

int	ft_fputstr_fd(int fd, char *str)
{
	int	count;
	int	err;

	count = 0;
	if (!str)
		str = "(null)";
	while (str[count])
	{
		err = write(fd, &str[count], 1);
		if (count == INT_MAX || err == -1)
			return (-1);
		count++;
	}
	return (count);
}

int	ft_fputnbr_fd(int fd, int n)
{
	char	num[12];
	int		i;
	long	nb;

	nb = n;
	if (nb == 0)
		return (write(fd, "0", 1));
	if (nb < 0)
	{
		write(fd, "-", 1);
		nb = -nb;
	}
	i = 10;
	num[11] = '\0';
	while (nb > 0)
	{
		num[i--] = '0' + (nb % 10);
		nb /= 10;
	}
	return (write(fd, &num[i + 1], 10 - i));
}

int	identify_type_fd(int fd, char c, va_list ap)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_fputchar_fd(fd, va_arg(ap, int));
	else if (c == 's')
		count += ft_fputstr_fd(fd, va_arg(ap, char *));
	else if (c == 'd' || c == 'i')
		count += ft_fputnbr_fd(fd, va_arg(ap, int));
	else if (c == '%')
		count += write(fd, "%", 1);
	return (count);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		count;
	int		tmp;
	int		i;

	count = 0;
	i = 0;
	va_start(ap, format);
	while (format[i])
	{
		tmp = 0;
		if (format[i] == '%' && format[i + 1])
			tmp += identify_type_fd(fd, format[++i], ap);
		else if (!(format[i] == '%' && format[i + 1] == '\0'))
			tmp += write(fd, &format[i], 1);
		if (tmp == -1 || count > INT_MAX - tmp)
		{
			va_end(ap);
			return (-1);
		}
		count += tmp;
		i++;
	}
	va_end(ap);
	return (count);
}
