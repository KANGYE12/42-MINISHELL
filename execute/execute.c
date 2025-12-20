/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanye <kanye@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:58:49 by iisraa11          #+#    #+#             */
/*   Updated: 2025/12/20 19:25:01 by kanye            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

static void child_proccess(t_cmd *cmd, t_env *my_env, int prev_fd, int pipe_fd[2])
{
    char **envp_array;
    char *path;

    if (cmd->heredoc_fd != -1) // we have used heredoc
    {
        dup2(cmd->heredoc_fd, STDIN_FILENO); // now it reads from the pipe. FILENO --> standard input from the keyboard
        close(cmd->heredoc_fd);
    }
    envp_array = env_list_to_array(my_env);
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
    if (is_builtin(cmd->argv[0]))
    {
        exec_builtin(cmd, &my_env, 0);
        exit(0);
    }
    if (ft_strchr(cmd->argv[0], '/')) // si contiene '/' es ruta absoluta o relativa
        path = cmd->argv[0];
    else
        path = find_cmd_path(cmd->argv[0], my_env);
    if (!path)
    {
        printf("minishell: command not found: %s\n", cmd->argv[0]);
        free_double_ptr(envp_array);
        exit(127); // command not found number
    }
    execve(path, cmd->argv, envp_array);
    exit(1);
}

int executor(t_cmd *cmd_list, t_env **my_env)
{
    t_cmd *cmd;
    int prev_fd;
    int pipe_fd[2];
    pid_t pid;

    if (!cmd_list->argv || !cmd_list->argv[0])
        return 0;

    cmd = cmd_list;
    prev_fd = -1;

    if (cmd_list && !cmd_list->next && is_builtin(cmd_list->argv[0]))
    {
        exec_builtin(cmd_list, my_env, 1);
        return (0);
    }

    while (cmd)
    {
        if (cmd->next)
            pipe(pipe_fd);
        pid = fork();
        if (pid == 0)
            child_proccess(cmd, *my_env, prev_fd, pipe_fd);
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd->next)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        cmd = cmd->next;
    }
    while (wait(NULL) > 0)
        ;
    return (0);
}
