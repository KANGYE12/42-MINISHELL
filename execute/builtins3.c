/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isrguerr <isrguerr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 16:18:50 by iisraa11          #+#    #+#             */
/*   Updated: 2026/02/27 15:35:55 by isrguerr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

int	builtin_pwd(char **argv)
{
	char	cwd[4096];
	
	if (argv[1])
	{
		write(2, "pwd: too many arguments\n", 24);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
	}
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_env **my_env, int in_parent)
{
	char	**a;

	a = cmd->argv;
	if (!ft_strcmp(a[0], "echo"))
		return (builtin_echo(a));
	if (!ft_strcmp(a[0], "pwd"))
		return (builtin_pwd(a));
	if (!ft_strcmp(a[0], "cd") && in_parent)
		return (builtin_cd(a));
	if (!ft_strcmp(a[0], "export") && in_parent)
		return (builtin_export(a, my_env));
	if (!ft_strcmp(a[0], "unset") && in_parent)
		return (builtin_unset(a, my_env));
	if (!ft_strcmp(a[0], "env"))
		return (builtin_env(*my_env));
	if (!ft_strcmp(a[0], "exit") && in_parent)
		return (builtin_exit(a));
	return (127);
}
