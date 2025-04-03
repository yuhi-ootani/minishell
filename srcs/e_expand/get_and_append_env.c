/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_and_append_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:00:40 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 10:47:15 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_name(t_minishell *shell, const char *input)
{
	size_t	len;
	char	*name;

	if (!input)
		return (NULL);
	if (ft_isalpha(input[0]) || input[0] == '_')
		len = 1;
	else
		return (NULL);
	while (ft_isalnum(input[len]) || input[len] == '_')
		len++;
	name = (char *)ft_calloc((len + 1), sizeof(char));
	if (!name)
	{
		shell->exit_status = EXIT_FAILURE;
		return (NULL);
	}
	if (len > 0)
		ft_strncpy(name, input, len);
	name[len] = '\0';
	return (name);
}

bool	append_to_buffer(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src, size_t src_len)
{
	char	*new_buffer;
	size_t	old_size;
	size_t	new_size;

	if (expanded_str->index + src_len >= expanded_str->size)
	{
		old_size = expanded_str->size;
		new_size = expanded_str->index + src_len + 1;
		new_buffer = ft_realloc(expanded_str->buffer, old_size, new_size);
		if (!new_buffer)
		{
			shell->exit_status = EXIT_FAILURE;
			shell->exit_status = EXIT_FAILURE;
			return (false);
		}
		expanded_str->buffer = new_buffer;
		expanded_str->size = new_size;
	}
	ft_memcpy(expanded_str->buffer + expanded_str->index, src, src_len);
	expanded_str->index += src_len;
	expanded_str->buffer[expanded_str->index] = '\0';
	return (true);
}

bool	append_exit_status(t_minishell *shell, t_expanded_str *expanded_str)
{
	char	*exit_status;

	exit_status = ft_itoa(shell->exit_status);
	if (!exit_status)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	if (!append_to_buffer(shell, expanded_str, exit_status,
			ft_strlen(exit_status)))
	{
		free(exit_status);
		return (false);
	}
	free(exit_status);
	return (true);
}

bool	append_one_char(t_minishell *shell, t_expanded_str *s_expanded_str,
		const char *src_input, size_t *i)
{
	if (src_input[*i] == '\'' && !s_expanded_str->in_double_quote)
	{
		s_expanded_str->in_single_quote = !s_expanded_str->in_single_quote;
	}
	else if (src_input[*i] == '"' && !s_expanded_str->in_single_quote)
	{
		s_expanded_str->in_double_quote = !s_expanded_str->in_double_quote;
	}
	if (!append_to_buffer(shell, s_expanded_str, &src_input[*i], 1))
		return (false);
	(*i)++;
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
// 			tmp = strdup_except_quotes_util(splited_args[j]);
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

// void	expand_cmds(t_minishell *shell)
// {
// 	t_command	*current;
// 	char		**new_args;

// 	current = shell->commands;
// 	while (current)
// 	{
// 		new_args = expander(shell, current->args);
// 		ft_array_free(current->args);
// 		current->args = new_args;
// 		current = current->next;
// 	}
// }
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