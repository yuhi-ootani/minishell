

#include "../../include/minishell.h"

t_env	*env_duplication(char **envp_srcs)
{
	t_env	*new_env;
	t_env	*head_of_copied_env;
	char	**splited_env;

	head_of_copied_env = NULL;
	while (*envp_srcs)
	{
		splited_env = ft_split(*envp_srcs, '=');
		if (!splited_env || !splited_env[0])
		{
			envp_srcs++; // todo
			continue ;
		}
		new_env = (t_env *)malloc(sizeof(t_env));
		if (!new_env)
			break ; // todo
		new_env->name = strdup(splited_env[0]);
		new_env->value = NULL;
		if (splited_env[1])
			new_env->value = strdup(splited_env[1]);
		else if (strchr(*splited_env, '='))
			new_env->value = strdup("");
		new_env->next = head_of_copied_env;
		head_of_copied_env = new_env;
		ft_array_free(splited_env);
		envp_srcs++;
	}
	return (head_of_copied_env);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	*duped_env;
// 	t_env	*tmp;

// 	duped_env = env_dup(envp);
// 	while (duped_env)
// 	{
// 		printf("%s=%s\n", duped_env->name, duped_env->value);
// 		tmp = duped_env;
// 		duped_env = duped_env->next;
// 		free(tmp->name);
// 		free(tmp->value);
// 		free(tmp);
// 	}
// 	return (0);
// }
