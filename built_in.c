#include "minishell.h"


void builtin_commands(char **args, char **envp)
{
    if (ft_strcmp(args[0], "echo") == 0)
        ft_echo(args);
    else if (ft_strcmp(args[0], "pwd") == 0)
        ft_pwd();
    else if (ft_strcmp(args[0], "export") == 0)
        ft_export(args);
    else if (ft_strcmp(args[0], "unset") == 0)
        ft_unset(args[1]);
    else if (ft_strcmp(args[0], "env") == 0)
        ft_env(envp);
    else if (ft_strcmp(args[0], "exit") == 0)
        ft_exit();
    if (ft_strcmp(args[0], "cd") == 0)
    {
        ft_cd(args[1]);
        return 1;
    }
    return 0; 
}

void ft_echo(char **args)
{
    int i = 1;
    int new_line = 1;

    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        new_line = 0;
        i++;
    }

    while (args[i])
    {
        if (args[i][0] == '$')
        {
            char *env_value = getenv(args[i] + 1);
            if (env_value)
                printf("%s", env_value);
            else
                printf("");
        }
        else
            printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (new_line)
        printf("\n");
}


void ft_cd(char *path)
{
    char *home;

    if (!path || *path == '\0')
    {
        home = getenv("HOME");
        if (!home || *home == '\0')
        {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
        path = home;
    }

    if (chdir(path) != 0)
        perror("cd");
}


void ft_pwd()
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
}

void ft_export(char **args)
{
    char *var;
    char *value;

    if (!args[1] || strchr(args[1], '=') == NULL) // Ensure correct format
    {
        fprintf(stderr, "export: invalid format. Use VAR=value\n");
        return;
    }

    var = strtok(args[1], "=");    // Get variable name
    value = strtok(NULL, "=");     // Get value

    if (!var || !value)
    {
        fprintf(stderr, "export: invalid format. Use VAR=value\n");
        return;
    }

    if (setenv(var, value, 1) != 0) // Set the environment variable
        perror("export");
}

void ft_unset(char *var)
{
    if (!var)
    {
        fprintf(stderr, "unset: argument not found\n");
        return;
    }
    if (unsetenv(var) != 0)
        perror("unset");
}

void ft_env(char **envp)
{
    for (int i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
}

void ft_exit()
{
    exit(0);
}
