

#include "../includes/minishell.h"

void free_token_list(t_token *token_list)
{
    t_token *tmp;

    while (token_list)
    {
        tmp = token_list->next;
        if (token_list->str)
            free(token_list->str);
        free(token_list);
        token_list = tmp;
    }
}
