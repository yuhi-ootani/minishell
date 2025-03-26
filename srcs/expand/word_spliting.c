/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_spliting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:00:40 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/26 14:21:29 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	ft_array_nbr_dup(char **array_dst, char **array_src, size_t dst_start,
		size_t dst_end)
{
	size_t	dst_i;
	size_t	src_i;

	dst_i = dst_start;
	src_i = 0;
	if (!array_dst || !array_src)
		return (false);
	while (dst_i < dst_end && array_src[src_i])
	{
		array_dst[dst_i] = ft_strdup(array_src[src_i]);
		// malloc fail
		dst_i++;
		src_i++;
	}
	return (true);
}

char	**append_string_arrays(char **array1, char **array2)
{
	size_t	len1;
	size_t	len2;
	char	**new_array;

	len1 = 0;
	len2 = 0;
	if (array1)
		len1 = ft_array_count_str(array1);
	if (array2)
		len2 = ft_array_count_str(array2);
	new_array = (char **)malloc(sizeof(char *) * (len1 + len2 + 1));
	if (!new_array)
		return (NULL); // Allocation failed
	ft_array_nbr_dup(new_array, array1, 0, len1);
	ft_array_nbr_dup(new_array, array2, len1, len1 + len2);
	new_array[len1 + len2] = NULL;
	return (new_array);
}

char	*remove_quotes(const char *input)
{
	char	*result;
	size_t	j;
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	j = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double)
			in_single = !in_single;
		else if (*input == '"' && !in_single)
			in_double = !in_double;
		else
			result[j++] = *input;
		input++;
	}
	result[j] = '\0';
	return (result);
}

static char	**split_and_remove_quotes(char *str, const char *spaces)
{
	char	**splited_args;
	char	*tmp;
	size_t	j;

	if (str[0] != '"' && str[ft_strlen(str) - 1] != '"')
		splited_args = ft_split(str, spaces);
	else
		splited_args = ft_split(str, "");
	j = 0;
	while (splited_args && splited_args[j])
	{
		tmp = remove_quotes(splited_args[j]);
		if (tmp)
		{
			free(splited_args[j]);
			splited_args[j] = tmp;
		}
		j++;
	}
	return (splited_args);
}

void	append_expanded_argument(t_minishell *shell, char ***result, char *arg,
		const char *spaces)
{
	char	*expanded;
	char	**splited_args;
	char	**tmp_array;

	expanded = get_expanded_str(shell, arg);
	if (!expanded)
		return ;
	splited_args = split_and_remove_quotes(expanded, spaces);
	free(expanded);
	tmp_array = append_string_arrays(*result, splited_args);
	ft_array_free(splited_args);
	ft_array_free(*result);
	*result = tmp_array;
}

char	**expander(t_minishell *shell, t_command *cmd)
{
	size_t	i;
	char	**result;
	char	*spaces;
	char	**args;

	spaces = DELIMITERS;
	result = NULL;
	i = 0;
	args = cmd->args;
	while (args && args[i])
	{
		append_expanded_argument(shell, &result, args[i], spaces);
		i++;
	}
	return (result);
}

void	expand_commands(t_minishell *shell)
{
	t_command	*current;
	char		**new_args;

	current = shell->commands;
	while (current)
	{
		new_args = expander(shell, current);
		ft_array_free(current->args);
		current->args = new_args;
		current = current->next;
	}
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