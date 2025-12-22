/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:32 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:04:09 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* ------------------- ENVIRONMENT FUNCTIONS ------------------- */

/*
USER=kanye
HOME=/home/kanye
PATH=/usr/local/bin:/usr/bin:/bin
PWD=/home/kanye/minishell
*/

static t_env	*ft_lstnew(char *env)
{
	t_env	*node;
	char	*str_equal;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	str_equal = ft_strchr(env, '=');
	if (str_equal)
	{
		node->key = ft_substr(env, 0, str_equal - env);
		node->value = ft_strdup(str_equal + 1);
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
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

char	*get_env_value(t_env **env, char *key)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	free_env(t_env **env)
{
	t_env	*current;
	t_env	*temp;

	current = *env;
	while (current)
	{
		temp = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = temp;
	}
}

t_env	*init_env_list(char **envp)
{
	t_env	*env_list;
	t_env	*new;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		new = ft_lstnew(envp[i]);
		ft_lstadd_back(&env_list, new);
		i++;
	}
	return (env_list);
}
