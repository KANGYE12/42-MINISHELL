/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:41 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 18:11:12 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	add_to_argv(t_cmd *cmd, char *str, int *argc)
{
	char	**temp_argv;

	temp_argv = resize_argv(cmd->argv, *argc);
	if (!temp_argv)
	{
		perror("ERROR IN REALLOC");
		return (ERROR);
	}
	temp_argv[*argc] = ft_strdup(str);
	if (!temp_argv[*argc])
	{
		perror("minishell: malloc");
		free(temp_argv);
		return (ERROR);
	}
	temp_argv[*argc + 1] = NULL;
	free(cmd->argv);
	cmd->argv = temp_argv;
	(*argc)++;
	return (SUCCESS);
}

int	handle_infile(t_cmd *cmd, t_token *token)
{
	char	*temp;
	size_t	len;

	if (cmd->infile)
		free(cmd->infile);
	len = ft_strlen(token->next->str);
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
	{
		perror("MALLOC OF HANDLE_INFILE IS NULL");
		return (ERROR);
	}
	cmd->infile = temp;
	ft_strcpy(cmd->infile, token->next->str);
	return (SUCCESS);
}

void	handle_heredoc(t_cmd *cmd, t_heredoc *hdoc)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return ;
	}
	hdoc->fd = pipe_fd[1];
	read_heredoc_input(hdoc);
	close(pipe_fd[1]);
	if (cmd->infile)
	{
		free(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	cmd->heredoc_fd = pipe_fd[0];
}

int	handle_outfile(t_cmd *cmd, t_token *token, int append)
{
	char	*temp;
	size_t	len;

	if (cmd->outfile)
		free(cmd->outfile);
	len = ft_strlen(token->next->str);
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
	{
		perror("MALLOC OF HANDLE_OUTFILE IS NULL");
		return (ERROR);
	}
	cmd->outfile = temp;
	ft_strcpy(cmd->outfile, token->next->str);
	cmd->append = append;
	return (SUCCESS);
}

t_cmd	*parse_tokens_to_cmds(t_token *tokens, t_env **env, int last_exit)
{
	t_cmd	*cmd_list;
	t_cmd	*cmd;
	t_token	*current;
	int		argc;

	cmd_list = NULL;
	cmd = ft_lstnew_parse();
	current = tokens;
	argc = 0;
	if (!cmd)
		return (NULL);
	while (current)
	{
		if (current->type == TOKEN_WORD)
			add_to_argv(cmd, current->str, &argc);
		else if (current->type == TOKEN_PIPE)
			handle_pipe(&cmd_list, &cmd, &argc);
		else
			handle_redirections_and_heredoc(cmd, &current, env, last_exit);
		if (current)
			current = current->next;
	}
	ft_lstadd_back_parse(&cmd_list, cmd);
	return (cmd_list);
}
