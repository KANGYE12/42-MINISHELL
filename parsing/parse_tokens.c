/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:44 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:07:32 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* This function reads a line from the user and stores tokens in a linked list.
   It ignores completely the empty spaces. */
char	*reading_line(t_token **token_list)
{
	char	*line;

	line = readline("minishell> ");
	if (!line)
		return (NULL);
	if (line[0] == '\0')
		return (line);
	add_history(line);
	receive_line(line, token_list);
	return (line);
}

/* Function to classify the different token types that can be passed as arguments */
void	classify_tokens(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->str, "|") == 0)
			current->type = TOKEN_PIPE;
		else if (ft_strcmp(current->str, "<") == 0)
			current->type = TOKEN_REDIR_IN;
		else if (ft_strcmp(current->str, ">") == 0)
			current->type = TOKEN_REDIR_OUT;
		else if (ft_strcmp(current->str, ">>") == 0)
			current->type = TOKEN_APPEND;
		else if (ft_strcmp(current->str, "<<") == 0)
			current->type = TOKEN_HEREDOC;
		else
			current->type = TOKEN_WORD;
		current = current->next;
	}
}
