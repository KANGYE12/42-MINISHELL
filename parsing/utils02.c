/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils02.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 17:47:22 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 18:13:57 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*ft_lstnew_parse(void)
{
	t_cmd	*new_list;

	new_list = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_list)
		return (NULL);
	new_list->argv = NULL;
	new_list->infile = NULL;
	new_list->outfile = NULL;
	new_list->next = NULL;
	new_list->append = 0;
	new_list->heredoc_fd = -1;
	return (new_list);
}

void	ft_lstadd_back_parse(t_cmd **lst, t_cmd *new)
{
	t_cmd	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal_status = 130;
}

char	**resize_argv(char **old, int size)
{
	char	**new;
	int		i;

	i = 0;
	new = malloc(sizeof(char *) * (size + 2));
	if (!new)
		return (NULL);
	while (i < size)
	{
		new[i] = old[i];
		i++;
	}
	return (new);
}
