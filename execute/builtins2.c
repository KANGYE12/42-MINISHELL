/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:03 by iisraa11          #+#    #+#             */
/*   Updated: 2026/03/01 19:37:34 by iisraa11         ###   ########.fr       */
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

static int	is_valid_identifier(const char *s, int len)
{
	int	j;

	if (len == 0)
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	j = 1;
	while (j < len)
	{
		if (!ft_isalnum(s[j]) && s[j] != '_')
			return (0);
		j++;
	}
	return (1);
}

static int	export_one(char *arg, t_env **env_list)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		if (!is_valid_identifier(arg, ft_strlen(arg)))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			return (1);
		}
		return (0);
	}
	if (!is_valid_identifier(arg, eq - arg))
	{
		ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
		return (1);
	}
	key = ft_strndup(arg, eq - arg);
	value = ft_strdup(eq + 1);
	add_or_update_env(env_list, key, value);
	return (0);
}

int	builtin_export(char **argv, t_env **env_list)
{
	int	status;

	status = 0;
	argv++;
	while (*argv)
	{
		if (export_one(*argv, env_list))
			return (1);
		argv++;
	}
	return (status);
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
