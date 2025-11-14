#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_token *token_list;
    t_cmd   *cmd_list;
    t_env   *env_list;
    char    *line;
    int     last_exit_status = 0;

    (void)argv;
    if (argc != 1)
        return 0;

    // Initialize environment list
    env_list = init_env_list(envp);

    // Optional: print all environment variables (debug)
    printf("Environment variables:\n");
    t_env *tmp_env = env_list;
    while (tmp_env)
    {
        printf("  %s=%s\n", tmp_env->key, tmp_env->value);
        tmp_env = tmp_env->next;
    }
    printf("---------------------------------\n");

    // Main shell loop
    while (1)
    {
        token_list = NULL;
        line = reading_line(&token_list);

        if (!line)
        {
            if (feof(stdin)) // Ctrl+D
            {
                printf("exit\n");
                break;
            }
            continue;
        }

        // --- Parsing ---
        classify_tokens(token_list);
        merge_adjacent_words(&token_list);
        replace_tokens_variables(token_list, &env_list, last_exit_status);
        remove_quotes_from_list(token_list);

        // Print token list
        printf("\nToken list:\n");
        t_token *tmp = token_list;
        while (tmp)
        {
            printf("  [%s]\n", tmp->str);
            tmp = tmp->next;
        }

        // Check syntax
        if (check_syntax_tokens(token_list) == SYNTAX_ERROR)
        {
            error_message("MINISHELL SYNTAX ERROR", token_list);
            last_exit_status = 2;
        }
        else
        {
            // Convert tokens into commands
            cmd_list = parse_tokens_to_cmds(token_list);

            // Print command distribution
            printf("\nCommands distribution:\n");
            t_cmd *c = cmd_list;
            int cmd_index = 0;
            while (c)
            {
                printf("Command %d:\n", cmd_index);
                if (c->argv)
                {
                    for (int j = 0; c->argv[j]; j++)
                        printf("  argv[%d]: [%s]\n", j, c->argv[j]);
                }
                if (c->infile)
                    printf("  infile: [%s]\n", c->infile);
                if (c->outfile)
                    printf("  outfile: [%s] (append=%d)\n", c->outfile, c->append);
                c = c->next;
                cmd_index++;
            }

            // Execute commands
            last_exit_status = executor(cmd_list, env_list);

            // Free command list
            free_cmd_list(&cmd_list);
        }

        // Free tokens and line
        free_token_list(&token_list);
        free(line);
    }

    // Free environment list
    free_env(&env_list);

    return last_exit_status; //ensures shell to exit with the correct status of the last command executed
}
