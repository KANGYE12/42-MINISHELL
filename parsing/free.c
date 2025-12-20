/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:35 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:05:23 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_digit_str(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	error_message(char *message, t_token *token_list)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	(void)token_list;
}

void	free_token_list(t_token **token_list)
{
	t_token	*current;
	t_token	*temp;

	current = *token_list;
	while (current != NULL)
	{
		temp = current->next;
		free(current->str);
		free(current);
		current = temp;
	}
	*token_list = NULL;
}

void	free_cmd_list(t_cmd **cmd_list)
{
	t_cmd	*current;
	t_cmd	*temp;
	int		i;

	if (!cmd_list || !*cmd_list)
		return ;
	current = *cmd_list;
	while (current != NULL)
	{
		i = 0;
		temp = current->next;
		if (current->argv)
		{
			while (current->argv[i] != NULL)
			{
				free(current->argv[i]);
				i++;
			}
			free(current->argv);
		}
		if (current->infile)
			free(current->infile);
		if (current->outfile)
			free(current->outfile);
		if (current->heredoc_fd != -1)
			close(current->heredoc_fd);
		free(current);
		current = temp;
	}
	*cmd_list = NULL;
}
