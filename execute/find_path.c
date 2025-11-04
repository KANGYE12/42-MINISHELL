/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:14:18 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/04 10:12:40 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Search for the PATH variable in the environment.
 */
static char *search_path_in_env(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return envp[i] + 5;
        i++;
    }
    return NULL;
}
/*
 * Find the full path of a command by searching the PATH environment variable.
 */
char *find_cmd_path(char *cmd, char **envp)
{
    int i;
    char *path_env;
    char *full_path;
    char **paths;

    path_env = search_path_in_env(envp);
    if (!path_env)
        return NULL;
    
    paths = ft_split(path_env, ':');
    i = -1;
    while (paths[++i])
    {
        char *tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, X_OK) == 0)
        {
            free_double_ptr(paths);
            return full_path;
        }
        free(full_path);
    }
    free_double_ptr(paths);
    return NULL;
}