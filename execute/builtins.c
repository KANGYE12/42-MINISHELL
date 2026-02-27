/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isrguerr <isrguerr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:31:06 by iisraa11          #+#    #+#             */
/*   Updated: 2026/02/27 16:58:23 by isrguerr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_echo_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-' || !str[1])
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n' && str[i] != 'e')
			return (0);
		i++;
	}
	return (1);
}

static int	has_echo_flag(char *str, char flag)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] == flag)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_echo(char **argv)
{
	int		i;
	int		newline;
	char	*processed;

	i = 1;
	newline = 1;
	while (argv[i] && is_echo_flag(argv[i]))
	{
		if (has_echo_flag(argv[i], 'n'))
			newline = 0;
		i++;
	}
	while (argv[i])
	{
		processed = process_escape_sequences(argv[i]);
		write(1, processed, ft_strlen(processed));
		free(processed);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

int	builtin_exit(char **argv)
{
	int	status;

	status = 0;
	if (argv[1])
		status = ft_atoi(argv[1]);
	printf("exit\n");
	exit(status);
}

int	builtin_cd(char **argv)
{
	if (!argv[1])
	{
		printf("minishell: cd: missing argument\n");
		return (1);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
