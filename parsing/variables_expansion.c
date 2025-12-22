/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:58 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 20:31:55 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_dollar(char *str, int *i, t_env **env, int last_exit_status)
{
	char	*value;
	int		var_len;
	char	*var_name;

	if (str[*i] == '?')
	{
		value = ft_itoa(last_exit_status);
		(*i)++;
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		var_name = get_var_name(&str[*i], &var_len);
		value = dup_env_or_empty(get_env_value(env, var_name));
		free(var_name);
		(*i) += var_len;
	}
	else
	{
		value = ft_strdup("$");
		(*i)++;
	}
	return (value);
}

static void	handle_quotes(char c, int *single_quote, int *double_quote)
{
	if (c == '\'' && !*double_quote)
		*single_quote = !*single_quote;
	else if (c == '"' && !*single_quote)
		*double_quote = !*double_quote;
}

static char	*append_chunk(char *result, char *str, int start, int end)
{
	char	*chunk;
	char	*new_result;

	chunk = ft_substr(str, start, end - start);
	new_result = ft_strjoin_free_s1(result, chunk);
	free(chunk);
	return (new_result);
}

static void	append_expanded(char **res, char *str, int start, t_exp *e)
{
	char	*val;

	*res = append_chunk(*res, str, start, e->i++);
	val = expand_dollar(str, &e->i, e->env, e->last_exit_status);
	*res = ft_strjoin_free_s1(*res, val);
	free(val);
}

char	*expand_variables(char *str, t_env **env, int last_exit_status)
{
	char	*res;
	t_exp	e;

	res = ft_strdup("");
	e.i = 0;
	e.start = 0;
	e.quotes[0] = 0;
	e.quotes[1] = 0;
	e.env = env;
	e.last_exit_status = last_exit_status;
	while (str[e.i])
	{
		handle_quotes(str[e.i], &e.quotes[0], &e.quotes[1]);
		if (str[e.i] == '$' && !e.quotes[0]
			&& (e.i == 0 || str[e.i - 1] != '\\'))
		{
			append_expanded(&res, str, e.start, &e);
			e.start = e.i--;
		}
		e.i++;
	}
	res = append_chunk(res, str, e.start, e.i);
	free(str);
	return (res);
}
