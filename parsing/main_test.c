#include "../includes/minishell.h"

int main(int argc, char **argv)
{
    t_token *token_list;
    t_cmd *cmd_list;
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

        // Optional: classify tokens
        classify_tokens(token_list);

        // Print tokens for testing
        t_token *tmp = token_list;
        printf("Token list:\n");
        while (tmp)
        {
            printf("Token: '%s'\n", tmp->str);
            tmp = tmp->next;
        }

        // Check syntax
        if (check_syntax_tokens(token_list) == SYNTAX_ERROR)
        {
            error_message("MINISHELL SYNTAX ERROR", token_list);
        }
        else
        {
            // Convert tokens to commands
            cmd_list = parse_tokens_to_cmds(token_list);

            // Print command list for testing
            t_cmd *c = cmd_list;
            int i = 0;
            printf("\nCommands generated:\n");
            while (c)
            {
                printf("Command %d:\n", i);
                if (c->argv)
                {
                    for (int j = 0; c->argv[j]; j++)
                        printf("  argv[%d]: %s\n", j, c->argv[j]);
                }
                if (c->infile)
                    printf("  infile: %s\n", c->infile);
                if (c->outfile)
                    printf("  outfile: %s (append=%d)\n", c->outfile, c->append);
                c = c->next;
                i++;
            }

            // Free commands
            free_cmd_list(&cmd_list);
        }

        // Free tokens and line
        free_token_list(&token_list);
        free(line);
    }

    return 0;
}
