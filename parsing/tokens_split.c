/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:52 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 18:54:28 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Create a new token without linking it */
t_token	*ft_lstnew_token(char *str)
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
void	ft_lstadd_back_token(t_token **lst, t_token *new, int has_space)
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

/* Split the input line into tokens, handling quotes, operators, and words */
static t_token	*extract_next_token(char *line, int *i)
{
	if (line[*i] == '\'' || line[*i] == '"')
		return (extract_token_from_quote(line, i));
	else if (is_operator_char(line[*i]))
		return (extract_token_from_operator(line, i));
	else
		return (extract_token_from_word(line, i));
}

static void	skip_spaces(char *line, int *i, int *has_space)
{
	if (is_space(line[*i]))
	{
		*has_space = 1;
		while (is_space(line[*i]))
			(*i)++;
	}
}

void	receive_line(char *line_read, t_token **token_list)
{
	int		i;
	t_token	*new;
	int		has_space;

	i = 0;
	has_space = 0;
	new = NULL;
	while (line_read[i] != '\0')
	{
		skip_spaces(line_read, &i, &has_space);
		if (line_read[i] == '\0')
			break ;
		new = extract_next_token(line_read, &i);
		if (!new)
		{
			free_token_list(token_list);
			return ;
		}
		ft_lstadd_back_token(token_list, new, has_space);
		has_space = 0;
		new = NULL;
	}
}
