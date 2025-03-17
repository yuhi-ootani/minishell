

#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>

size_t	ft_array_count_str(char **array)
{
	size_t	count;

	count = 0;
	while (array && array[count])
	{
		count++;
	}
	return (count);
}

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

char	**expander(t_minishell *shell, char **args)
{
	size_t	i;
	char	**new_array;
	char	**result;
	char	**splited_args;
	char	*spaces;
	char	*expanded;
	char	*removed;

	// char	*no_quotes;
	spaces = delimiters;
	new_array = NULL;
	result = NULL;
	i = 0;
	while (args[i])
	{
		expanded = set_argument_for_expansion(args[i], shell->env);
		if (!expanded)
		{
			i++;
			continue ;
		}
		if (args[i][0] != '\"' && args[i][ft_strlen(args[i]) - 1] != '\"')
			splited_args = ft_split(expanded, spaces);
		else
			splited_args = ft_split(expanded, "");
		new_array = append_string_arrays(result, splited_args);
		ft_array_free(splited_args);
		free(result);
		result = new_array;
		i++;
	}
	i = 0;
	while (result[i])
	{
		removed = remove_quotes(result[i]);
		if (removed)
		{
			free(result[i]);
			result[i] = removed;
		}
		i++;
	}
	return (result);
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
