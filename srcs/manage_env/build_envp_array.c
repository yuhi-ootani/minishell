

#include "../../include/minishell.h"

char	*ft_strjoin_three(char *str1, char *str2, char *str3)
{
	char	*tmp;
	char	*result;

	if (!str1 || !str2 || !str3)
		return (NULL);
	tmp = ft_strjoin(str1, str2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, str3);
	free(tmp);
	if (!result)
		return (NULL);
	return (result);
}

char	**build_envp_array(t_env *env)
{
	int		count;
	char	**envp_array;
	t_env	*tmp;

	count = 0;
	count = ft_lstsize(env); // remove name bonus
	envp_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp_array)
		return (NULL); // todo
	count = 0;
	tmp = env;
	while (tmp)
	{
		envp_array[count] = ft_strjoin_three(tmp->name, "=", tmp->value);
		if (envp_array[count])
			return (NULL); // to do
		count++;
	}
	envp_array[count] = NULL;
	return (envp_array);
}
