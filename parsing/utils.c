#include "../includes/minishell.h"


int is_space(char c)
{
    if (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r')
        return 1;
    return 0;
}    


int is_operator_char(char c)
{
    if (c == '|' || c == '<' || c == '>')
        return 1;
    return 0;    
}