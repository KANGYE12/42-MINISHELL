#include "../includes/minishell.h"

//ENVIROMENTS FUNCTIONS

/*
USER=kanye
HOME=/home/kanye
PATH=/usr/local/bin:/usr/bin:/bin
PWD=/home/kanye/minishell
*/
static t_env	*ft_lstnew(char *env)
{
	t_env	*node;
    char    *str_equal; // the string from = to the end

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
    str_equal = ft_strchr(env, '=');
    if(str_equal)
    {
        node->key = ft_substr(env, 0, str_equal - env);
        node->value = ft_strdup(str_equal + 1); // to not include =
    }
    else
    {
        node->key = ft_strdup(env);
        node->value = ft_strdup("");
    }
    node->next = NULL;
	return (node);
}

static void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temporal;

	if (!lst || !new)
		return ;
	if (*lst == NULL) 
	{
		*lst = new;
		return ;
	}
	temporal = *lst;
	while (temporal->next != NULL)
		temporal = temporal->next;
	temporal->next = new;
}

//get the enviroment value based on the key
char   *get_env_value(t_env **env, char *key)
{
    t_env *current;

    current = (*env);
    while(current)
    {
        if(ft_strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;    
    }
    return NULL;
}

void free_env(t_env **env)
{
    t_env *temporal;
    t_env *current;

    current = (*env);
    while (current)
    {
        temporal = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = temporal;
    }
}


t_env *init_env_list(char **envp)
{
    t_env *env_list;
    t_env *new;

    int i = 0;
    env_list = NULL;
    while (envp[i])
    {
        new = ft_lstnew(envp[i]);
        ft_lstadd_back(&env_list, new);
        i++;
    }
    return env_list;
}