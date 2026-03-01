/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 16:49:13 by isrguerr          #+#    #+#             */
/*   Updated: 2026/03/01 19:25:46 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_escaped_char(char c, char *out)
{
	if (c == 'n')
		*out = '\n';
	else if (c == 't')
		*out = '\t';
	else if (c == '\\')
		*out = '\\';
	else if (c == 'a')
		*out = '\a';
	else if (c == 'b')
		*out = '\b';
	else if (c == 'r')
		*out = '\r';
	else if (c == 'v')
		*out = '\v';
	else
		return (0);
	return (1);
}

char	*process_escape_sequences(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	escaped;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1]
			&& get_escaped_char(str[i + 1], &escaped))
		{
			result[j++] = escaped;
			i += 2;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

int	builtin_env(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		write(1, tmp->key, ft_strlen(tmp->key));
		write(1, "=", 1);
		write(1, tmp->value, ft_strlen(tmp->value));
		write(1, "\n", 1);
		tmp = tmp->next;
	}
	return (0);
}

int	builtin_cd(char **argv, t_env **env)
{
	char	*target;

	if (argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!argv[1])
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		target = argv[1];
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

void	delete_env_node(t_env **env_list, t_env *node, t_env *prev)
{
	if (prev)
		prev->next = node->next;
	else
		*env_list = node->next;
	free(node->key);
	free(node->value);
	free(node);
}
