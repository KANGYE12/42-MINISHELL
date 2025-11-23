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

int is_delimiter(char c)
{
    if (is_space(c) || is_operator_char(c) || c == '\'' || c == '"')
        return (1);
    return (0);
}

void free_double_ptr(char **ptr)
{
    int i;

    i = 0;
    while (ptr[i])
    {
        free(ptr[i]);
        i++;
    }
    free(ptr);
}

int is_var_char(char c)
{
    if(ft_isalnum(c) || c == '_')
        return 1;
    return 0;    
}

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_signal_status = 130;
}