#include "../includes/minishell.h"

//Check if the syntax of the line is correctly written
int syntax_check(t_token *token_list)
{
    t_token *current;
    
    current = token_list;
    if(!current)
        return SUCCESS; //There is nothing to check
    if(current->type == TOKEN_PIPE) //The pipe cannot be at the begining, it must be something behind it
        return SYNTAX_ERROR;
        
    while(current != NULL)
    {
        if ((current->type == TOKEN_PIPE || current->type == TOKEN_REDIR_OUT ||
            current->type == TOKEN_REDIR_IN || current->type == TOKEN_APPEND ||
            current->type == TOKEN_HEREDOC) &&
            (!current->next || current->next->type != TOKEN_WORD))
            return (SYNTAX_ERROR);
        current = current->next;    
    }
    return SUCCESS;
}