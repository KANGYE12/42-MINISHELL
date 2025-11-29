/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:03 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/24 16:42:27 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int builtin_export(char **argv, t_env **env_list)
{
    int i;
    size_t key_len;
    char *key;
    char *value;
    t_env *curr;
    
    i = 1;
    while (argv[i])
    {
        char *eq = ft_strchr(argv[i], '=');
        if (!eq)
        {
            i++;
            continue; // ignorar si no tiene '='
        }

        key_len = eq - argv[i];
        key = strndup(argv[i], key_len);
        value = ft_strdup(eq + 1);

        // buscar si ya existe
        curr = *env_list;
        int found = 0;
        while (curr)
        {
            if (ft_strcmp(curr->key, key) == 0)
            {
                free(curr->value);
                curr->value = value;
                found = 1;
                break;
            }
            curr = curr->next;
        }
        if (!found)
        {
            t_env *new = malloc(sizeof(t_env));
            new->key = key;
            new->value = value;
            new->next = *env_list;
            *env_list = new;
        }
        else
            free(key);
        i++;
    }
    return (0);
}

int builtin_unset(char **argv, t_env **env_list)
{
    t_env *prev;
    t_env *curr;
    int i;

    prev = NULL;
    curr = *env_list;
    i = 1;
    while (argv[i])
    {
        curr = *env_list;
        prev = NULL;
        while (curr)
        {
            if (ft_strcmp(curr->key, argv[i]) == 0)
            {
                if (prev)
                    prev->next = curr->next;
                else
                    *env_list = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        i++;
    }
    return (0);
}

int exec_builtin(t_cmd *cmd, t_env **my_env, int in_parent)
{
    char **a = cmd->argv;
    
    if (!ft_strcmp(a[0], "echo"))
        return builtin_echo(a);

    if (!ft_strcmp(a[0], "pwd"))
        return builtin_pwd();
        
    if (!ft_strcmp(a[0], "cd") && in_parent)
        return builtin_cd(a);
        
    if (!ft_strcmp(a[0], "export") && in_parent)
        return builtin_export(a, my_env);
        
    if (!ft_strcmp(a[0], "unset") && in_parent)
        return builtin_unset(a, my_env);
        
    if (!ft_strcmp(a[0], "env"))
        return builtin_env(*my_env);
        
    if (!ft_strcmp(a[0], "exit") && in_parent)
        return builtin_exit(a);

    return (0);
}
