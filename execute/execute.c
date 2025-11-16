/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:58:49 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/16 19:42:28 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void child_proccess(t_cmd *cmd, char **envp, int prev_fd, int pipe_fd[2])
{
    char *path;
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
    path = find_cmd_path(cmd->argv[0], envp);
    if (!path)
    {
        printf("minishell: command not found: %s\n", cmd->argv[0]);
        exit(1);
    }
    if (cmd->infile || cmd->outfile)
        handle_redirections(cmd);
    execve(path, cmd->argv, envp);
    exit(1);
}

void executor(t_cmd *cmd_list, char **envp)
{
    t_cmd *cmd;
    int prev_fd;
    int pipe_fd[2];
    pid_t pid;

    cmd = cmd_list;
    prev_fd = -1;
    while (cmd)
    {
        if (cmd->next)
            pipe(pipe_fd);
        pid = fork();
        if (pid == 0)
            child_proccess(cmd, envp, prev_fd, pipe_fd);
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd->next)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        cmd = cmd->next;
    }
    while (wait(NULL) > 0);
}
