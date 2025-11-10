#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_token *token_list;
    t_cmd   *cmd_list;
    t_env   *env_list;
    char    *line;
    // --- ¡CAMBIO! Añadido para manejar $? ---
    int     last_exit_status = 0; 

    (void)argv;
    if (argc != 1)
        return 0;

    // ✅ Initialize environment list
    env_list = init_env_list(envp);

    // ✅ Print all environment variables (tu debug)
    printf("Environment variables:\n");
    t_env *tmp_env = env_list;
    while (tmp_env)
    {
        // (Cambié tu printf un poco para que sea más claro)
        printf("  %s=%s\n", tmp_env->key, tmp_env->value);
        tmp_env = tmp_env->next;
    }
    printf("---------------------------------\n");

    // Main shell loop
    while (1)
    {
        token_list = NULL; // reset token list each command
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

        // --- INICIO DE PARSING ---
        classify_tokens(token_list);
        merge_adjacent_words(&token_list);

        // --- ¡CAMBIO! Se pasa last_exit_status ---
        // Paso 4: Expansión de variables ($VAR, $?)
        replace_tokens_variables(token_list, &env_list, last_exit_status);

        // --- ¡CAMBIO! PASO 5 AÑADIDO ---
        // Limpieza final de comillas
        remove_quotes_from_list(token_list);
        // --- FIN DE PARSING ---


        // Print tokens for testing (tu debug)
        printf("Token list:\n");
        t_token *tmp = token_list;
        while (tmp)
        {
            printf("Token: [%s]\n", tmp->str); // Añadí [] para ver strings vacíos
            tmp = tmp->next;
        }

        // Check syntax
        if (check_syntax_tokens(token_list) == SYNTAX_ERROR)
        {
            error_message("MINISHELL SYNTAX ERROR", token_list);
            // --- ¡CAMBIO! Actualiza el estado en caso de error ---
            last_exit_status = 2; // 2 es estándar para error de sintaxis
        }
        else
        {
            // Convert tokens to commands
            cmd_list = parse_tokens_to_cmds(token_list);

            // Print command list for testing (tu debug)
            t_cmd *c = cmd_list;
            int i = 0;
            printf("\nCommands generated:\n");
            while (c)
            {
                printf("Command %d:\n", i);
                if (c->argv)
                {
                    for (int j = 0; c->argv[j]; j++)
                        printf("  argv[%d]: [%s]\n", j, c->argv[j]);
                }
                // ... (tus otros printf de infile/outfile) ...
                c = c->next;
                i++;
            }

            // --- ¡CAMBIO! Pasa tu env_list y actualiza el estado ---
            last_exit_status = executor(cmd_list, env_list);
            
            free_cmd_list(&cmd_list);
        }

        // --- ¡CAMBIO! Descomentado para evitar leaks ---
        free_token_list(&token_list);
        free(line);
    }

    // ✅ Free environment list before exiting
    free_env(&env_list);

    // --- ¡CAMBIO! Devuelve el último estado ---
    return (last_exit_status);
}