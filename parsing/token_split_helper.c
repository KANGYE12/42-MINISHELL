/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:30:35 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 18:54:08 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Extract a token from quoted string */
t_token	*extract_token_from_quote(char *line, int *i)
{
	char	*copy;
	int		start;
	char	quote;

	quote = line[*i];
	start = *i;
	(*i)++;
	while (line[*i] != '\0' && line[*i] != quote)
		(*i)++;
	if (line[*i] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (NULL);
	}
	copy = ft_substr(line, start, (*i - start) + 1);
	(*i)++;
	return (ft_lstnew_token(copy));
}

/* Extract a token from operator: <, >, <<, >> */
t_token	*extract_token_from_operator(char *line, int *i)
{
	char	*copy;

	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
	{
		copy = ft_substr(line, *i, 2);
		(*i) += 2;
		return (ft_lstnew_token(copy));
	}
	copy = ft_substr(line, *i, 1);
	(*i)++;
	return (ft_lstnew_token(copy));
}

/* Extract a token from a word */
t_token	*extract_token_from_word(char *line, int *i)
{
	int		start;
	char	*copy;

	start = *i;
	while (!is_delimiter(line[*i]) && line[*i] != '\0')
		(*i)++;
	copy = ft_substr(line, start, *i - start);
	return (ft_lstnew_token(copy));
}
