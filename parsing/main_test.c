
#include "../includes/minishell.h"

int main(int argc, char **argv)
{
    t_token *token_list;
    t_cmd *cmd_list;
    char    *line;

    token_list = NULL;
    (void)argv;
    if(argc != 1)
        return 0;
    while(1){
        line = reading_line(&token_list);
        t_token *tmp = token_list;
        printf("Token list:\n");
        while (tmp)
        {
            printf("Token: '%s'\n", tmp->str);
            tmp = tmp->next;
        }
        parse_tokens_to_cmds(token_list); //We classify and parse to cmd commands
        if(syntax_check(token_list) == SYNTAX_ERROR)
            printf("ERROR SYNTAX\n");
        free_token_list(token_list);
        token_list = NULL;
        free(line);
    }
    return 0;
}