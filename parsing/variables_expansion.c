#include "../includes/minishell.h"


static char *ft_strjoin_free_s1(char *s1, char *s2)
{
    char *new_str;

    if(s2 == NULL)
        new_str = ft_strjoin(s1, "");
    else
        new_str = ft_strjoin(s1, s2);
    free(s1);
    return (new_str);
}

static char *get_var_name(const char *str, int *len)
{
    int i;

    i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    *len = i;
    return ft_substr(str, 0, i);
}


char *expand_variables(char *str, t_env **env, int last_exit_status)
{
    char    *result;
    int     i;
    int     start;
    int     single_quote;
    int     double_quote ;

    i = 0;
    single_quote = 0;
    double_quote = 0;
    start = 0;
    result = ft_strdup("");
    while (str[i])
    {
        if (str[i] == '\'' && !double_quote)
            single_quote = !single_quote;
        else if (str[i] == '"' && !single_quote)
            double_quote = !double_quote;
        if (str[i] == '$' && !single_quote && str[i-1] != '\\') //since only double quotes or no quote allows expansion
        {
            char *temporal = ft_substr(str, start, i - start);
            result = ft_strjoin_free_s1(result, temporal);
            free(temporal);       
            i++;
            if (str[i] == '?') //return last exit status
            {
                char *status = ft_itoa(last_exit_status);
                result = ft_strjoin_free_s1(result, status);
                free(status);
                i++;
            }
            else if (ft_isalpha(str[i]) || str[i] == '_') //search for the enviroment value
            {
                int var_len = 0;
                char *var_name = get_var_name(&str[i], &var_len); //like HOME
                char *value = get_env_value(env, var_name); //not free this variable since we did not add any malloc in this function
                result = ft_strjoin_free_s1(result, value);
                free(var_name);
                i += var_len;
            }
            else //if the enviroment is not a valid one
                result = ft_strjoin_free_s1(result, "$");
            start = i;
        }
        else
            i++;
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
            current->str = expand_variables(current->str, env, last_exit_status);
        current = current->next;
    }
}