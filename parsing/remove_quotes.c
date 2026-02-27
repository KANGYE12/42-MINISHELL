/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isrguerr <isrguerr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:46 by kanye             #+#    #+#             */
/*   Updated: 2026/02/27 16:04:52 by isrguerr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_quotes_and_escapes(char *delimiter, int *i,
		int *single_quote, int *double_quote)
{
	if (delimiter[*i] == '\'' && !*double_quote)
	{
		*single_quote = !*single_quote;
		(*i)++;
		return (1);
	}
	if (delimiter[*i] == '"' && !*single_quote)
	{
		*double_quote = !*double_quote;
		(*i)++;
		return (1);
	}
	if (delimiter[*i] == '\\')
	{
		if (!*single_quote && !*double_quote)
		{
			(*i)++;
			return (1);
		}
		else if (*double_quote && (delimiter[*i + 1] == '$'
				|| delimiter[*i + 1] == '\\' || delimiter[*i + 1] == '"'))
		{
			(*i)++;
			return (1);
		}
		return (0);
	}
	return (0);
}

static int	unclosed_quotes(char *new_delimiter, int single_quote,
		int double_quote)
{
	if (single_quote || double_quote)
	{
		free(new_delimiter);
		return (0);
	}
	return (1);
}

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
		if (!handle_quotes_and_escapes(delimiter, &i, &single_quote,
				&double_quote))
			new_delimiter[j++] = delimiter[i++];
	}
	if (unclosed_quotes(new_delimiter, single_quote, double_quote) == 0)
		return (NULL);
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
