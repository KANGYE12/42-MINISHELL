/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:52 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:08:40 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Create a new token without linking it */
static t_token	*ft_lstnew(char *str)
{
	t_token	*new_list;

	new_list = (t_token *)malloc(sizeof(t_token));
	if (!new_list)
		return (NULL);
	new_list->str = str;
	new_list->next = NULL;
	new_list->prev = NULL;
	return (new_list);
}

/* Add a token at the end of the list, with optional space flag */
static void	ft_lstadd_back(t_token **lst, t_token *new, int has_space)
{
	t_token	*temporal;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temporal = *lst;
	while (temporal->next != NULL)
		temporal = temporal->next;
	temporal->next = new;
	new->prev = temporal;
	new->has_space = has_space;
}

/* Extract a token from quoted string */
static t_token	*extract_token_from_quote(char *line, int *i)
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
	return (ft_lstnew(copy));
}

/* Extract a token from operator: <, >, <<, >> */
static t_token	*extract_token_from_operator(char *line, int *i)
{
	char	*copy;

	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
	{
		copy = ft_substr(line, *i, 2);
		(*i) += 2;
		return (ft_lstnew(copy));
	}
	copy = ft_substr(line, *i, 1);
	(*i)++;
	return (ft_lstnew(copy));
}

/* Extract a token from a word */
static t_token	*extract_token_from_word(char *line, int *i)
{
	int		start;
	char	*copy;

	start = *i;
	while (!is_delimiter(line[*i]) && line[*i] != '\0')
		(*i)++;
	copy = ft_substr(line, start, *i - start);
	return (ft_lstnew(copy));
}

/* Split the input line into tokens, handling quotes, operators, and words */
void	receive_line(char *line_read, t_token **token_list)
{
	int		i;
	t_token	*new;
	int		has_space;

	i = 0;
	has_space = 0;
	while (line_read[i] != '\0')
	{
		if (is_space(line_read[i]))
		{
			has_space = 1;
			i++;
			while (is_space(line_read[i]))
				i++;
			continue ;
		}
		else if (line_read[i] == '\'' || line_read[i] == '"')
			new = extract_token_from_quote(line_read, &i);
		else if (is_operator_char(line_read[i]))
			new = extract_token_from_operator(line_read, &i);
		else
			new = extract_token_from_word(line_read, &i);
		if (!new)
		{
			free_token_list(token_list);
			return ;
		}
		ft_lstadd_back(token_list, new, has_space);
		has_space = 0;
		new = NULL;
	}
}
