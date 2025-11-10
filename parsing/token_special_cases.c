
#include "../includes/minishell.h"

//This file is to handle the special cases of parsing where the user can put in the terminal


// for echo "hello_world"'hello' example cases--> So we are merging different follwing quotes so there is not a space between arguments
void merge_adjacent_words(t_token **token_list)
{
    t_token *current;
    char *join_string;
    t_token *temporal;

    if(!token_list || !(*token_list))
        return;
    current = (*token_list);    
    while(current && current->next)
    {
        temporal = current->next;
        if(current->type == TOKEN_WORD && current->next->type == TOKEN_WORD && !temporal->has_space)
        {
            join_string = ft_strjoin(current->str, current->next->str);
            free(current->str);
            current->str = join_string;
            current->next = temporal->next;
            if(temporal->next)
                temporal->next->prev = current; //we are deleting the node we are joining
            free(temporal->str);
            free(temporal);    
        }
        else
            current = current->next;
    }    
}







