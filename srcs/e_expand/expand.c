/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:09:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/02 18:57:58 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

bool	init_expanded_str(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input)
{
	expanded_str->size = ft_strlen(src_input) + 1;
	expanded_str->index = 0;
	expanded_str->in_single_quote = false;
	expanded_str->in_double_quote = false;
	expanded_str->buffer = ft_calloc(sizeof(char), expanded_str->size);
	if (!expanded_str->buffer)
	{
		shell->exit_status = EXIT_FAILURE;
		return (false);
	}
	return (true);
}

char	*get_expanded_str(t_minishell *shell, const char *src_input)
{
	t_expanded_str	expanded_str;
	size_t			i;

	if (!src_input)
		return (NULL);
	if (!init_expanded_str(shell, &expanded_str, src_input))
		return (NULL);
	i = 0;
	while (src_input[i])
	{
		if (src_input[i] == '$' && !expanded_str.in_single_quote)
		{
			i++;
			if (!handle_dollar(shell, &expanded_str, src_input, &i))
				return (free(expanded_str.buffer), NULL);
		}
		else
		{
			if (!append_one_char(shell, &expanded_str, src_input, &i))
				return (free(expanded_str.buffer), NULL);
		}
	}
	return (expanded_str.buffer);
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
	if (!quote_removal_args(shell, result))
	{
		ft_array_free(result);
		return (NULL);
	}
	return (result);
}
//used in 2 functions

// static int	append_to_buffer(t_expanded_str *expanded_str, const char *src,
// 		size_t src_len)
// {
// 	char	*new_buffer;

// 	if (expanded_str->index + src_len >= expanded_str->size)
// 	{
// 		expanded_str->size = expanded_str->index + src_len + 1;
// 		new_buffer = realloc(expanded_str->buffer, expanded_str->size);
// 		if (!new_buffer)
// 			return (-1);
// 		expanded_str->buffer = new_buffer;
// 	}
// 	ft_memcpy(expanded_str->buffer + expanded_str->index, src, src_len);
// 	expanded_str->index += src_len;
// 	expanded_str->buffer[expanded_str->index] = '\0';
// 	return (0);
// }
