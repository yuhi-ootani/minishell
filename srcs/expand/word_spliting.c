/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_spliting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:00:40 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/28 14:35:49 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	remove_quotes_and_copy(char *dst, const char *src)
{
	bool	in_single;
	bool	in_double;
	size_t	i;

	i = 0;
	in_single = false;
	in_double = false;
	while (*src)
	{
		if (*src == '\'' && !in_double)
			in_single = !in_single;
		else if (*src == '"' && !in_single)
			in_double = !in_double;
		else
		{
			dst[i] = *src;
			i++;
		}
		src++;
	}
	dst[i] = '\0';
}

char	*remove_quotes(t_minishell *shell, const char *input)
{
	char	*result;

	result = (char *)ft_calloc(sizeof(char), ft_strlen(input) + 1);
	if (!result)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	remove_quotes_and_copy(result, input);
	return (result);
}

static bool	quote_removal(t_minishell *shell, char **args)
{
	char	*tmp;
	size_t	i;

	i = 0;
	while (args && args[i])
	{
		tmp = remove_quotes(shell, args[i]);
		if (tmp)
		{
			free(args[i]);
			args[i] = tmp;
		}
		else
			return (false);
		i++;
	}
	return (true);
}

char	**word_splitting(t_minishell *shell, char *str)
{
	char	**splited_args;

	if (str[0] != '"' && str[ft_strlen(str) - 1] != '"')
		splited_args = ft_split(str, DELIMITERS);
	else
		splited_args = ft_split(str, "");
	if (!splited_args)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	return (splited_args);
}

char	**expander(t_minishell *shell, char *arg)
{
	char	*expanded_str;
	char	**result;

	expanded_str = get_expanded_str(shell, arg);
	if (!expanded_str)
		return (NULL);
	result = word_splitting(shell, expanded_str);
	free(expanded_str);
	if (!result)
		return (NULL);
	if (!quote_removal(shell, result))
	{
		ft_array_free(result);
		return (NULL);
	}
	return (result);
}

char	**create_expanded_args(t_minishell *shell, t_command *cmd)
{
	size_t	i;
	char	**result;
	char	**expanded;
	char	**joined_array;

	result = NULL;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		expanded = expander(shell, cmd->args[i]);
		if (!expanded)
			return (NULL);
		joined_array = ft_array_join(result, expanded);
		ft_array_free(expanded);
		ft_array_free(result);
		result = joined_array;
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

bool	expand_all_cmd_args(t_minishell *shell)
{
	t_command	*current;
	char		**new_args;

	current = shell->commands;
	while (current)
	{
		if (current->args && current->args[0])
		{
			new_args = create_expanded_args(shell, current);
			if (!new_args)
				return (false);
			ft_array_free(current->args);
			current->args = new_args;
		}
		current = current->next;
	}
	return (true);
}

// // Example usage
// int	main(void)
// {
// 	char	*arr[] = {"Hello", "World",
// 			"\" word                          split   \"", " Cut   Them     ",
// 			" popo   ", NULL};
// 	char	**merged;

// 	merged = expander(arr);
// 	if (!merged)
// 	{
// 		perror("append_string_arrays failed");
// 		return (1);
// 	}
// 	// Print merged array
// 	for (size_t i = 0; merged[i] != NULL; i++)
// 		printf("%s$\n", merged[i]);
// 	// NOTE: We only allocated 'merged' (the array of pointers).
// 	//       We did NOT allocate each string. So we only free 'merged' itself.
// 	free(merged);
// 	return (0);
// }

// char	**expander(t_minishell *shell, char **args)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	**result;
// 	char	**splited_args;
// 	char	*spaces;
// 	char	**tmp_array;
// 	char	*tmp;

// 	// char	*no_quotes;
// 	spaces = DELIMITERS;
// 	result = NULL;
// 	i = 0;
// 	while (args && args[i])
// 	{
// 		tmp = get_expanded_str(shell, args[i]);
// 		if (!tmp)
// 		{
// 			i++;
// 			continue ;
// 		}
// 		if (args[i][0] != '\"' && args[i][ft_strlen(args[i]) - 1] != '\"')
// 			splited_args = ft_split(tmp, spaces);
// 		else
// 			splited_args = ft_split(tmp, "");
// 		free(tmp);
// 		j = 0;
// 		while (splited_args[j])
// 		{
// 			tmp = remove_quotes(splited_args[j]);
// 			if (tmp)
// 			{
// 				free(splited_args[j]);
// 				splited_args[j] = tmp;
// 			}
// 			j++;
// 		}
// 		tmp_array = append_string_arrays(result, splited_args);
// 		ft_array_free(splited_args);
// 		ft_array_free(result);
// 		result = tmp_array;
// 		i++;
// 	}
// 	return (result);
// }