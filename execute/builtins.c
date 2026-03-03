/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isrguerr <isrguerr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:31:06 by iisraa11          #+#    #+#             */
/*   Updated: 2026/03/03 12:38:29 by isrguerr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	has_echo_flag(char *str, char flag)
{
	int	i;
	int	found;

	if (!str || str[0] != '-' || !str[1])
		return (0);
	i = 1;
	found = 0;
	while (str[i])
	{
		if (str[i] != 'n' && str[i] != 'e')
			return (0);
		if (str[i] == flag)
			found = 1;
		i++;
	}
	return (found);
}

static void	echo_parse_flags(char **argv, int *i, int *nl, int *esc)
{
	while (argv[*i] && (has_echo_flag(argv[*i], 'n')
			|| has_echo_flag(argv[*i], 'e')))
	{
		if (has_echo_flag(argv[*i], 'n'))
			*nl = 0;
		if (has_echo_flag(argv[*i], 'e'))
			*esc = 1;
		(*i)++;
	}
}

int	builtin_echo(char **argv)
{
	int		i;
	int		newline;
	int		escape;
	char	*processed;

	i = 1;
	newline = 1;
	escape = 0;
	echo_parse_flags(argv, &i, &newline, &escape);
	while (argv[i])
	{
		if (escape)
			processed = process_escape_sequences(argv[i]);
		else
			processed = ft_strdup(argv[i]);
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

static int	is_all_digits(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **argv)
{
	int	status;

	status = 0;
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (argv[1])
	{
		if (!is_all_digits(argv[1]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			exit(2);
		}
		status = ft_atoi(argv[1]);
	}
	printf("exit\n");
	exit(status);
}
