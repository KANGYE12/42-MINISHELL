#include "../includes/minishell.h"


static char *clean_quotes_from_str(char *str)
{
    char *new_str;
    int i = 0;
    int j = 0;
    int in_single = 0;
    int in_double = 0;

    new_str = (char *)malloc(ft_strlen(str) + 1);
    if (!new_str)
        return (NULL);
    
    while (str[i])
    {
        if (str[i] == '\'' && !in_double)
        {
            in_single = !in_single; // Cambia de estado
            i++; // Salta la comilla, no la copies
            continue;
        }
        if (str[i] == '"' && !in_single)
        {
            in_double = !in_double; // Cambia de estado
            i++; // Salta la comilla, no la copies
            continue;
        }
        new_str[j++] = str[i++]; // Copia el carácter
    }
    new_str[j] = '\0';
    free(str); // Libera la cadena original
    return (new_str);
}

void remove_quotes_from_list(t_token *token_list)
{
    t_token *current = token_list;
    while (current)
    {
        if (current->type == TOKEN_WORD)
        {
            current->str = clean_quotes_from_str(current->str);
        }
        current = current->next;
    }
}