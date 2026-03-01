/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:31:06 by iisraa11          #+#    #+#             */
/*   Updated: 2026/03/01 13:52:39 by iisraa11         ###   ########.fr       */
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

int	builtin_cd(char **argv, t_env **env)
{
	char	*target;

	if (argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!argv[1])
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		target = argv[1];
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
