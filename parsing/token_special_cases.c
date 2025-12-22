/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_special_cases.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:49 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:08:17 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* This file handles special cases in parsing where the user input may
   contain adjacent quoted strings that should be merged. */

/* For example: echo "hello_world"'hello' → merge into a single argument. */
void	merge_adjacent_words(t_token **token_list)
{
	t_token	*current;
	char	*join_string;
	t_token	*temporal;

	if (!token_list || !(*token_list))
		return ;
	current = (*token_list);
	while (current && current->next)
	{
		temporal = current->next;
		if (current->type == TOKEN_WORD && current->next->type == TOKEN_WORD
			&& !temporal->has_space)
		{
			join_string = ft_strjoin(current->str, current->next->str);
			free(current->str);
			current->str = join_string;
			current->next = temporal->next;
			if (temporal->next)
				temporal->next->prev = current;
			free(temporal->str);
			free(temporal);
		}
		else
			current = current->next;
	}
}
