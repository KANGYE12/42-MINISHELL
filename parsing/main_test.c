/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:38 by kanye             #+#    #+#             */
/*   Updated: 2025/12/21 20:37:54 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_status = 0;

static void	update_signal_status(int *last_exit_status)
{
	if (g_signal_status != 0)
	{
		*last_exit_status = g_signal_status;
		g_signal_status = 0;
	}
}

static char	*read_line_or_exit(t_token **token_list)
{
	char	*line;

	*token_list = NULL;
	line = reading_line(token_list);
	if (!line && isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	return (line);
}

static int	process_tokens(t_token *token_list, t_env **env_list,
		int last_exit_status)
{
	t_cmd	*cmd_list;
	int		status;

	status = last_exit_status;
	classify_tokens(token_list);
	merge_adjacent_words(&token_list);
	replace_tokens_variables(token_list, env_list, last_exit_status);
	remove_quotes_from_list(token_list);
	if (check_syntax_tokens(token_list) == SYNTAX_ERROR)
	{
		error_message("syntax error near unexpected token", token_list);
		status = 2;
	}
	else
	{
		cmd_list = parse_tokens_to_cmds(token_list, env_list,
				last_exit_status);
		if (cmd_list)
			status = executor(cmd_list, env_list);
		free_cmd_list(&cmd_list);
	}
	free_token_list(&token_list);
	return (status);
}

static int	shell_loop_iteration(t_env **env_list, int last_exit_status)
{
	t_token	*token_list;
	char	*line;

	update_signal_status(&last_exit_status);
	line = read_line_or_exit(&token_list);
	if (!line)
		return (-1);
	if (line[0] == '\0')
	{
		free(line);
		return (last_exit_status);
	}
	last_exit_status = process_tokens(token_list, env_list, last_exit_status);
	free(line);
	return (last_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	int		last_exit_status;
	int		status;

	(void)argv;
	if (argc != 1)
		return (0);
	env_list = init_env_list(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	last_exit_status = 0;
	while (1)
	{
		status = shell_loop_iteration(&env_list, last_exit_status);
		if (status == -1)
			break ;
		last_exit_status = status;
	}
	free_env(&env_list);
	return (last_exit_status);
}
