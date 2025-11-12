/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:14:18 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/12 21:04:08 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


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
