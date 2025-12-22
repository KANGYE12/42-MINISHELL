/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:03 by iisraa11          #+#    #+#             */
/*   Updated: 2025/12/22 16:21:54 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	add_or_update_env(t_env **env_list, char *key, char *value)
{
	t_env	*curr;
	t_env	*new;

	curr = *env_list;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = value;
			free(key);
			return ;
		}
		curr = curr->next;
	}
	new = malloc(sizeof(t_env));
	new->key = key;
	new->value = value;
	new->next = *env_list;
	*env_list = new;
}

int	builtin_export(char **argv, t_env **env_list)
{
	int		i;
	char	*eq;
	char	*key;
	char	*value;

	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		key = ft_strndup(argv[i], eq - argv[i]);
		value = ft_strdup(eq + 1);
		add_or_update_env(env_list, key, value);
		i++;
	}
	return (0);
}

static void	delete_env_node(t_env **env_list, t_env *node, t_env *prev)
{
	if (prev)
		prev->next = node->next;
	else
		*env_list = node->next;
	free(node->key);
	free(node->value);
	free(node);
}

int	builtin_unset(char **argv, t_env **env_list)
{
	t_env	*prev;
	t_env	*curr;
	int		i;

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
				delete_env_node(env_list, curr, prev);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
	return (0);
}
