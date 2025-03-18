

#include "../../include/minishell.h"

// static char	*get_env_value(char *value, char *env_src)
// {
// 	if (strchr(env_src, '='))
// 	{
// 		if (value)
// 			return (strdup(value));
// 		else
// 			return (strdup(""));
// 	}
// 	else
// 		return (NULL);
// }

t_env	*env_duplication(char **envp_srcs)
{
	t_env	*new_env;
	t_env	*head_of_copied_env;
	char	**splited_env;

	head_of_copied_env = NULL;
	while (*envp_srcs)
	{
		splited_env = ft_split(*envp_srcs, "=");
		if (!splited_env || !splited_env[0])
		{
			envp_srcs++; // todo
			continue ;
		}
		new_env = create_new_env_util(splited_env[0], splited_env[1], NULL);
		if (!new_env)
			break ; // todo
		env_add_back_util(&head_of_copied_env, new_env);
		ft_array_free(splited_env);
		envp_srcs++;
	}
	return (head_of_copied_env);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	*duped_env;
// 	t_env	*tmp;
// 	char	**envp_array;

// 	duped_env = env_duplication(envp);
// 	if (!duped_env)
// 	{
// 		printf("Error: env_dup() returned NULL.\n");
// 		return (1);
// 	}
// 	// Debug: Print env list before processing
// 	// printf("Debug: Printing linked list before conversion\n");
// 	// tmp = duped_env;
// 	// while (tmp)
// 	// {
// 	// 	printf("%s = %s\n", tmp->name, tmp->value);
// 	// 	tmp = tmp->next;
// 	// }
// 	// printf("\nNow converting to array...\n");
// 	envp_array = build_envp_array(duped_env);
// 	if (!envp_array)
// 	{
// 		printf("Error: build_envp_array() returned NULL.\n");
// 		return (1);
// 	}
// 	printf("\nFinal envp_array output:\n");
// 	for (int i = 0; envp_array[i]; i++)
// 		printf("%s\n", envp_array[i]);
// 	return (0);
// }
