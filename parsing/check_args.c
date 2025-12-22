/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:29 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:03:02 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_syntax_tokens(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (token_list == NULL)
		return (SUCCESS);
	else if (current->type == TOKEN_PIPE)
		return (SYNTAX_ERROR);
	while (current->next != NULL)
	{
		if (current->type != TOKEN_WORD && current->next->type != TOKEN_WORD)
			return (SYNTAX_ERROR);
		current = current->next;
	}
	if (current->type != TOKEN_WORD)
		return (SYNTAX_ERROR);
	return (SUCCESS);
}
