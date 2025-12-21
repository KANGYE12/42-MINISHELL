/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_expansion02.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:11:04 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 20:33:47 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_free_s1(char *s1, char *s2)
{
	char	*new_str;

	if (s2 == NULL)
		new_str = ft_strjoin(s1, "");
	else
		new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}

char	*get_var_name(const char *str, int *len)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

void	replace_tokens_variables(t_token *token_list, t_env **env,
		int last_exit_status)
{
	t_token	*current;

	current = token_list;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			current->str = expand_variables(current->str,
					env, last_exit_status);
		current = current->next;
	}
}

char	*dup_env_or_empty(const char *s)
{
	char	*res;

	if (s)
		res = ft_strdup(s);
	else
		res = ft_strdup("");
	return (res);
}
