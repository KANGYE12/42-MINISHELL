/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 17:45:31 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 20:35:05 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	read_heredoc_input(t_heredoc *hdoc)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, hdoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (hdoc->expansion)
			line = expand_variables(line, hdoc->env, hdoc->last_exit_status);
		write(hdoc->fd, line, ft_strlen(line));
		write(hdoc->fd, "\n", 1);
		free(line);
	}
}

static void	handle_single_heredoc(t_token **current, t_cmd *cmd)
{
	if ((*current)->type == TOKEN_REDIR_IN)
		handle_infile(cmd, *current);
	else if ((*current)->type == TOKEN_REDIR_OUT)
		handle_outfile(cmd, *current, 0);
	else if ((*current)->type == TOKEN_APPEND)
		handle_outfile(cmd, *current, 1);
}

void	handle_redirections_and_heredoc(t_cmd *cmd, t_token **current,
		t_env **env, int last_exit_status)
{
	t_heredoc	hdoc;
	char		*delimiter;
	int			expansion;

	expansion = 1;
	handle_single_heredoc(current, cmd);
	if ((*current)->type == TOKEN_HEREDOC)
	{
		if (ft_strchr((*current)->next->str, '\'')
			|| ft_strchr((*current)->next->str, '"'))
			expansion = 0;
		delimiter = clean_quotes_from_str((*current)->next->str);
		if (!delimiter)
			return (ft_putstr_fd("minishell: syntax error: \
unclosed delimiter\n", 2));
		hdoc.delimiter = delimiter;
		hdoc.expansion = expansion;
		hdoc.env = env;
		hdoc.last_exit_status = last_exit_status;
		handle_heredoc(cmd, &hdoc);
		free(delimiter);
		(*current) = (*current)->next;
	}
	else
		(*current) = (*current)->next;
}

void	handle_pipe(t_cmd **cmd_list, t_cmd **new_cmd, int *argc)
{
	ft_lstadd_back_parse(cmd_list, *new_cmd);
	*new_cmd = ft_lstnew_parse();
	*argc = 0;
}
