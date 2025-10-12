
#include "../includes/minishell.h"

int main(int argc, char **argv)
{
    t_token *token_list;
    char *line;

    (void)argv;
    if (argc != 1)
        return 0;

    while (1)
    {
        token_list = NULL;  // reset the token list for each command
        line = reading_line(&token_list);

        if (!line) // Ctrl+D or empty input
        {
            printf("exit\n");
            break;
        }

        // Optional: classify tokens (if your function is ready)
        classify_tokens(token_list);

        // Print tokens for testing
        t_token *tmp = token_list;
        printf("Token list:\n");
        while (tmp)
        {
            printf("Token: '%s'\n", tmp->str);
            tmp = tmp->next;
        }

        // Free token list
        tmp = token_list;
        while (tmp)
        {
            t_token *next = tmp->next;
            free(tmp->str); // free token string
            free(tmp);      // free token node
            tmp = next;
        }

        free(line); // free the input line
    }

    return 0;
}