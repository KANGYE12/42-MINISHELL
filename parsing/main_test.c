
#include "../includes/minishell.h"

int main(int argc, char **argv)
{
    t_token *token_list;

    token_list = NULL;
    if(argc != 2)
        return 0;
    while(1){
        reading_line(&token_list);
        t_token *tmp = token_list;
        printf("Token list:\n");
        while (tmp)
        {
            printf("Token: '%s'\n", tmp->str);
            tmp = tmp->next;
        }
    }
    return 0;

}