/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:58:49 by iisraa11          #+#    #+#             */
/*   Updated: 2025/10/31 13:14:57 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void exec_single(t_cmd *cmd, char **envp)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        char *path = find_cmd_path(cmd->argv[0], envp);
        if (!path)
        {
            printf("minishell: command not found: %s\n", cmd->argv[0]);
            exit(1);
        }
        execve(path, cmd->argv, envp);
        perror("execve");
        exit(1);
    }
    else
        waitpid(pid, NULL, 0);
}

void executor(t_cmd *cmd_list, char **envp)
{
    if (!cmd_list)
        return;

    if (cmd_list->next == NULL)
    {
        exec_single(cmd_list, envp);
        return;
    }
}