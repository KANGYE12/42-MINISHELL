/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:59:38 by kanye             #+#    #+#             */
/*   Updated: 2025/12/20 19:24:26 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>

int	g_signal_status = 0;
int	main(int argc, char **argv, char **envp)
{
	t_token	*token_list;
	t_cmd	*cmd_list;
	t_env	*env_list;
	char	*line;
	int		last_exit_status;

	last_exit_status = 0;
	(void)argv;
	if (argc != 1)
		return (0);
	env_list = init_env_list(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		if (g_signal_status != 0)
		{
			last_exit_status = g_signal_status;
			g_signal_status = 0;
		}
		token_list = NULL;
		line = reading_line(&token_list);
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", 2);
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		classify_tokens(token_list);
		merge_adjacent_words(&token_list);
		replace_tokens_variables(token_list, &env_list, last_exit_status);
		remove_quotes_from_list(token_list);
		if (check_syntax_tokens(token_list) == SYNTAX_ERROR)
		{
			error_message("syntax error near unexpected token", token_list);
			last_exit_status = 2;
		}
		else
		{
			cmd_list = parse_tokens_to_cmds(token_list, &env_list, last_exit_status);
			if (cmd_list)
				last_exit_status = executor(cmd_list, &env_list);
			free_cmd_list(&cmd_list);
		}
		free_token_list(&token_list);
		free(line);
	}
	free_env(&env_list);
	return (last_exit_status);
}
