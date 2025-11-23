#include "../includes/minishell.h"


void    error_message(char *message, t_token *token_list)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(message, 2);
    ft_putstr_fd("\n", 2); 
    
    if (token_list)
        free_token_list(&token_list);
}

void free_token_list(t_token **token_list)
{
    t_token *current;
    t_token *temporal;

    current = *token_list;
    while(current != NULL)
    {
        temporal = current->next;
        free(current->str);
        free(current);
        current = temporal;
    }
    *token_list = NULL;
}

void free_cmd_list(t_cmd **cmd_list)
{
    t_cmd *current;
    t_cmd *temporal;
    int i;

    if (!cmd_list || !*cmd_list)
        return;
    current = *cmd_list;
    while (current != NULL)
    {
        i = 0;
        temporal = current->next;
        if (current->argv)
        {
            while (current->argv[i] != NULL)
            {
                free(current->argv[i]);
                i++;
            }
            free(current->argv);
        }
        if (current->infile)
            free(current->infile);
        if (current->outfile)
            free(current->outfile);
        if (current->heredoc_fd != -1)
            close(current->heredoc_fd);
        free(current);
        current = temporal;
    }
    *cmd_list = NULL;
}
