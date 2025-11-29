/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:14:18 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/24 16:26:48 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//Convert the linked list of environment variables into an array of strings in the format KEY=VALUE.
char **env_list_to_array(t_env *env_list)
{
    t_env *current;
    char **env_array;
    char *temp_str;
    int count;
    int i;

    count = 0;
    i = 0;
    current = env_list;
    while (current)
    {
        count++;
        current = current->next;
    }
    env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);
    current = env_list;
    while (current)
    {
        temp_str = ft_strjoin(current->key, "=");
        env_array[i] = ft_strjoin(temp_str, current->value);
        free(temp_str);
        current = current->next;
        i++;
    }
    env_array[i] = NULL;
    return (env_array);
}

//Find the full path of a command by searching the PATH environment variable.
char *find_cmd_path(char *cmd, t_env *my_env)
{
	int i;
	char *path_env;
	char *full_path;
	char **paths;

	path_env = get_env_value(&my_env, "PATH");
	if (!path_env)
		return NULL;
	paths = ft_split(path_env, ':'); //we split it and try to find the correct path where our command can exist(through a loop)
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
