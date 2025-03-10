/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:16:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/01 15:45:53 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *remove_quotes(const char *input)
{
    size_t len = strlen(input);
    char *result = malloc(len + 1);  // Výsledný řetězec může být stejně dlouhý jako vstup
    if (!result)
        return NULL;

    size_t j = 0;  // Index pro výsledný řetězec
    bool in_single_quotes = false;
    bool in_double_quotes = false;

    for (size_t i = 0; input[i]; i++)
    {
        if (input[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            continue;
        }
        if (input[i] == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            continue;  // Dvojité uvozovky nekopírujeme
        }
        result[j++] = input[i];
    }
    result[j] = '\0';  // Ukončíme výsledný řetězec
    return result;
}


// Expanduje proměnné, ale ignoruje je uvnitř jednoduchých uvozovek
char *expand_variables(const char *input)
{
    char *result = malloc(strlen(input) + 1);
    if (!result)
    {
        return NULL;
    }

    size_t j = 0;  // Index pro výsledný řetězec
    bool in_single_quotes = false;  // Stav uvnitř jednoduchých uvozovek
    bool in_double_quotes = false;  // Stav uvnitř dvojitých uvozovek

    for (size_t i = 0; input[i]; i++)
    {
        if (input[i] == '\'' && !in_double_quotes)
        {
            // Přepínáme stav uvnitř jednoduchých uvozovek
            in_single_quotes = !in_single_quotes;
            result[j++] = input[i];  // Zachováme jednoduché uvozovky
            continue;
        }
        if (input[i] == '"' && !in_single_quotes)
        {
            // Přepínáme stav uvnitř dvojitých uvozovek
            in_double_quotes = !in_double_quotes;
            result[j++] = input[i];  // Zachováme dvojité uvozovky
            continue;
        }
        if (input[i] == '$' && !in_single_quotes)
        {
            // Zpracování proměnné (pouze mimo jednoduché uvozovky)
            i++;
            if (input[i] == '?')
            {
                // Expanze $? (není implementováno)
                strcat(result, "$?");
                j += 2;
            }
            else
            {
                size_t var_len = 0;
                while (isalnum(input[i + var_len]) || input[i + var_len] == '_')
                {
                    var_len++;
                }
                if (var_len > 0)
                {
                    char var_name[var_len + 1];
                    strncpy(var_name, &input[i], var_len);
                    var_name[var_len] = '\0';
                    char *var_value = getenv(var_name);
                    if (var_value)
                    {
                        strcat(result, var_value);
                        j += strlen(var_value);
                    }
                    i += var_len - 1;
                }
                else
                {
                    result[j++] = '$';
                }
            }
        }
        else
        {
            // Kopírujeme všechny ostatní znaky
            result[j++] = input[i];
        }
    }
    result[j] = '\0';  // Ukončíme výsledný řetězec
    return result;
}
char *expand_argument(const char *input)
{
    // Nejprve expandujeme proměnné, přičemž se respektují uvozovky – uvnitř
    // jednoduchých uvozovek proměnné neexpandujeme.
    char *expanded = expand_variables(input);
    if (!expanded)
        return NULL;

    // Poté odstraníme všechny uvozovky
    char *no_quotes = remove_quotes(expanded);
    free(expanded);
    return no_quotes;
}


// // Zpracuje argument: odstraní uvozovky a expanduje proměnné
// char *expand_argument(const char *input)
// {
//     char *no_quotes = remove_quotes(input);
//     if (!no_quotes)
//     {
//         return NULL;
//     }

//     char *expanded = expand_variables(no_quotes);
//     free(no_quotes);

//     return expanded;
// }

// Projde všechny příkazy a jejich argumenty a provede expanzi
void expand_commands(t_command *command_list)
{
    t_command *current = command_list;

    while (current)
    {
        for (size_t i = 0; current->args[i]; i++)
        {
            // Expandujte každý argument
            char *expanded_arg = expand_argument(current->args[i]);
            if (expanded_arg)
            {
                free(current->args[i]);  // Uvolněte původní argument
                current->args[i] = expanded_arg;  // Nahraďte expandovaným argumentem
            }
            else
            {
                // Pokud expanze selže, zachovejte původní argument
                fprintf(stderr, "Error expanding argument: %s\n", current->args[i]);
            }
        }
        current = current->next;
    }
}

