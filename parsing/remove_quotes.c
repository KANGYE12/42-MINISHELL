/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:46 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:07:54 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*clean_quotes_from_str(char *delimiter)
{
	char	*new_delimiter;
	int		i;
	int		j;
	int		single_quote;
	int		double_quote;

	new_delimiter = malloc(ft_strlen(delimiter) + 1);
	if (!new_delimiter)
		return (NULL);
	i = 0;
	j = 0;
	single_quote = 0;
	double_quote = 0;
	while (delimiter[i] != '\0')
	{
		if (delimiter[i] == '\'' && !double_quote)
		{
			single_quote = !single_quote;
			i++;
			continue ;
		}
		if (delimiter[i] == '"' && !single_quote)
		{
			double_quote = !double_quote;
			i++;
			continue ;
		}
		if (delimiter[i] == '\\')
		{
			if (!single_quote && !double_quote)
				i++;
			else if (double_quote)
			{
				if (delimiter[i + 1] == '$' || delimiter[i + 1] == '\\'
					|| delimiter[i + 1] == '"')
					i++;
			}
		}
		new_delimiter[j++] = delimiter[i++];
	}
	if (single_quote || double_quote)
	{
		free(new_delimiter);
		return (NULL);
	}
	new_delimiter[j] = '\0';
	return (new_delimiter);
}

void	remove_quotes_from_list(t_token *token_list)
{
	t_token	*current;
	char	*temporal;

	current = token_list;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			temporal = current->str;
			current->str = clean_quotes_from_str(current->str);
			free(temporal);
		}
		current = current->next;
	}
}
