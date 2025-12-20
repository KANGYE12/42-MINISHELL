/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:41 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:06:49 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_cmd	*ft_lstnew(void)
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

static void	ft_lstadd_back(t_cmd **lst, t_cmd *new)
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

static int	add_to_argv(t_cmd *new_cmd, char *str, int *args)
{
	char	**temp_argv;
	int		i;

	i = 0;
	temp_argv = malloc(sizeof(char *) * (*args + 2));
	if (!temp_argv)
	{
		perror("ERROR IN REALLOC");
		return (ERROR);
	}
	while (i < *args)
	{
		temp_argv[i] = new_cmd->argv[i];
		i++;
	}
	temp_argv[i] = ft_strdup(str);
	if (!temp_argv[i])
	{
		perror("minishell: malloc");
		free(temp_argv);
		return (ERROR);
	}
	temp_argv[i + 1] = NULL;
	if (new_cmd->argv)
		free(new_cmd->argv);
	new_cmd->argv = temp_argv;
	(*args)++;
	return (SUCCESS);
}

static int	handle_infile(t_cmd *new_cmd, t_token *current_token)
{
	char	*temp_infile;
	size_t	len;

	if (new_cmd->infile)
		free(new_cmd->infile);
	len = ft_strlen(current_token->next->str);
	temp_infile = malloc(sizeof(char) * (len + 1));
	if (!temp_infile)
	{
		perror("MALLOC OF HANDLE_INFILE IS NULL");
		return (ERROR);
	}
	new_cmd->infile = temp_infile;
	ft_strcpy(new_cmd->infile, current_token->next->str);
	return (SUCCESS);
}

static void	handle_heredoc(t_cmd *new_cmd, char *delimiter, int expansion,
	t_env **env, int last_exit_status)
{
	char	*line;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return ;
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		if (expansion)
			line = expand_variables(line, env, last_exit_status);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	if (new_cmd->infile)
	{
		free(new_cmd->infile);
		new_cmd->infile = NULL;
	}
	if (new_cmd->heredoc_fd != -1)
		close(new_cmd->heredoc_fd);
	new_cmd->heredoc_fd = pipe_fd[0];
}

static int	handle_outfile(t_cmd *new_cmd, t_token *current_token, int append)
{
	char	*temp_outfile;
	size_t	len;

	if (new_cmd->outfile)
		free(new_cmd->outfile);
	len = ft_strlen(current_token->next->str);
	temp_outfile = malloc(sizeof(char) * (len + 1));
	if (!temp_outfile)
	{
		perror("MALLOC OF HANDLE_OUTFILE IS NULL");
		return (ERROR);
	}
	new_cmd->outfile = temp_outfile;
	ft_strcpy(new_cmd->outfile, current_token->next->str);
	new_cmd->append = append;
	return (SUCCESS);
}

t_cmd	*parse_tokens_to_cmds(t_token *token_list, t_env **env, int last_exit_status)
{
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;
	t_token	*current;
	int		argc;
	char	*delimiter;
	int		expansion;

	cmd_list = NULL;
	new_cmd = ft_lstnew();
	current = token_list;
	argc = 0;
	expansion = 1;
	if (!new_cmd)
		return (NULL);
	while (current)
	{
		if (current->type == TOKEN_WORD)
			add_to_argv(new_cmd, current->str, &argc);
		else if (current->type == TOKEN_REDIR_IN)
		{
			handle_infile(new_cmd, current);
			current = current->next;
		}
		else if (current->type == TOKEN_REDIR_OUT)
		{
			handle_outfile(new_cmd, current, 0);
			current = current->next;
		}
		else if (current->type == TOKEN_APPEND)
		{
			handle_outfile(new_cmd, current, 1);
			current = current->next;
		}
		else if (current->type == TOKEN_PIPE)
		{
			ft_lstadd_back(&cmd_list, new_cmd);
			new_cmd = ft_lstnew();
			argc = 0;
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			if (ft_strchr(current->next->str, '\'')
				|| ft_strchr(current->next->str, '"'))
				expansion = 0;
			delimiter = clean_quotes_from_str(current->next->str);
			if (!delimiter)
			{
				ft_putstr_fd("minishell: syntax error: unclosed delimiter\n", 2);
				free_cmd_list(&cmd_list);
				free_cmd_list(&new_cmd);
				return (NULL);
			}
			handle_heredoc(new_cmd, delimiter, expansion, env, last_exit_status);
			free(delimiter);
			current = current->next;
		}
		if (current)
			current = current->next;
	}
	ft_lstadd_back(&cmd_list, new_cmd);
	return (cmd_list);
}
