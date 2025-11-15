#include "../includes/minishell.h"


static char *ft_strjoin_free_s1(char *s1, char *s2)
{
    char *new_str;

    new_str = ft_strjoin(s1, s2);
    free(s1);
    return (new_str);
}

static char *get_var_name(const char *str, int *len)
{
    int i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    *len = i;
    return ft_substr(str, 0, i);
}


char *expand_variables(char *str, t_env **env, int last_exit_status)
{
    char    *result;
    int     i = 0;
    int     start = 0;
    int     in_single = 0;
    int     in_double = 0;

    result = ft_strdup("");
    while (str[i])
    {

        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '"' && !in_single)
            in_double = !in_double;
        if (str[i] == '$' && !in_single)
        {
            char *chunk = ft_substr(str, start, i - start);
            result = ft_strjoin_free_s1(result, chunk);
            free(chunk);
            
            i++;
            if (str[i] == '?')
            {
                char *status = ft_itoa(last_exit_status);
                result = ft_strjoin_free_s1(result, status);
                free(status);
                i++;
            }
            else if (ft_isalpha(str[i]) || str[i] == '_')
            {
                int var_len = 0;
                char *var_name = get_var_name(&str[i], &var_len);
                char *value = get_env_value(env, var_name); 
                
                result = ft_strjoin_free_s1(result, value ? value : "");
                free(var_name);
                i += var_len;
            }
            else
            {
                result = ft_strjoin_free_s1(result, "$");
            }
            
            start = i;
        }
        else
        {
            i++;
        }
    }
 
    char *last_chunk = ft_substr(str, start, i - start);
    result = ft_strjoin_free_s1(result, last_chunk);
    free(last_chunk);

    free(str);
    return (result);
}


void replace_tokens_variables(t_token *token_list, t_env **env, int last_exit_status)
{
    t_token *current;

    current = token_list;
    while (current)
    {
        if (current->type == TOKEN_WORD)
        {
            current->str = expand_variables(current->str, env, last_exit_status);;
        }
        current = current->next;
    }
}