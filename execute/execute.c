/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:58:49 by iisraa11          #+#    #+#             */
/*   Updated: 2025/12/22 16:19:53 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	setup_fds(t_cmd *cmd, int prev_fd, int pipe_fd[2])
{
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (cmd->infile || cmd->outfile)
		handle_redirections(cmd);
}

static void	child_proccess(t_cmd *cmd, t_env *my_env, int prev_fd,
		int pipe_fd[2])
{
	char	**envp_array;
	char	*path;

	setup_fds(cmd, prev_fd, pipe_fd);
	envp_array = env_list_to_array(my_env);
	if (is_builtin(cmd->argv[0]))
	{
		exec_builtin(cmd, &my_env, 0);
		exit(0);
	}
	if (ft_strchr(cmd->argv[0], '/'))
		path = cmd->argv[0];
	else
		path = find_cmd_path(cmd->argv[0], my_env);
	if (!path)
	{
		printf("minishell: command not found: %s\n", cmd->argv[0]);
		free_double_ptr(envp_array);
		exit(127);
	}
	execve(path, cmd->argv, envp_array);
	exit(1);
}

static int	execute_single_builtin(t_cmd *cmd, t_env **my_env)
{
	int	save_stdin;
	int	save_stdout;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	handle_redirections(cmd);
	exec_builtin(cmd, my_env, 1);
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdin);
	close(save_stdout);
	return (0);
}

static int	execute_child_pipeline(t_cmd *cmd, t_env *my_env, int prev_fd,
		int *pipe_fd)
{
	pid_t	pid;

	if (cmd->next)
		pipe(pipe_fd);
	pid = fork();
	if (pid == 0)
		child_proccess(cmd, my_env, prev_fd, pipe_fd);
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
		return (pipe_fd[0]);
	return (-1);
}

int	executor(t_cmd *cmd_list, t_env **my_env)
{
	t_cmd	*cmd;
	int		prev_fd;
	int		pipe_fd[2];

	if (!cmd_list || !cmd_list->argv || !cmd_list->argv[0])
		return (0);
	cmd = cmd_list;
	prev_fd = -1;
	if (!cmd->next && is_builtin(cmd->argv[0]))
		return (execute_single_builtin(cmd, my_env));
	while (cmd)
	{
		prev_fd = execute_child_pipeline(cmd, *my_env, prev_fd, pipe_fd);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}
