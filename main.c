
#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    char input[1024];
    char *args[10];

    while (1)
    {
        printf("$> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Odstraní nový řádek

        int i = 0;
        char *token = strtok(input, " ");
        while (token)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        builtin_commands(args, envp);
    }
    return 0;
}
