/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:58:49 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/17 00:14:00 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//change each of the nodes into a double pointer
char **env_list_to_array(t_env *env_list)
{
    t_env   *current;
    char    **env_array;
    char    *temp_str;
    int     count;
    int     i;

    count = 0;
    i = 0;
    current = env_list; //get the amount of nodes to know how to fit the double array
    while (current)
    {
        count++;
        current = current->next;
    }
    env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);
    current = env_list;
    while (current)
    {
        temp_str = ft_strjoin(current->key, "=");
        env_array[i] = ft_strjoin(temp_str, current->value);
        free(temp_str);
        current = current->next;
        i++;
    }
    env_array[i] = NULL;
    return (env_array);
}

//must to be modified to free all later
static int exec_single(t_cmd *cmd, t_env *my_env)
{
    pid_t   pid;
    char    **envp_array;
    int     status;  
    int     exit_code;
    char    *path;

    exit_code = 0;
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    if (pid == 0)
    {
        if(cmd->heredoc_fd != -1)// we have used heredoc
        {
            dup2(cmd->heredoc_fd, STDIN_FILENO); //now it reads from the pipe. FILENO --> standard input from the keyboard
            close(cmd->heredoc_fd);
        }
        envp_array = env_list_to_array(my_env); 
        path = find_cmd_path(cmd->argv[0], my_env); 
        if (!path)
        {
            printf("minishell: command not found: %s\n", cmd->argv[0]);
            exit(1);
        }
        if (cmd->infile || cmd->outfile)
            handle_redirections(cmd);
        execve(path, cmd->argv, my_env);
        perror("execve");
        free(path);
        free_double_ptr(envp_array);
        exit(126); //command found but cannot execute due lack of permissions
    }
    else //for the parent process
    {
        waitpid(pid, &status, 0); 
        if (WIFEXITED(status)) //child terminated normally
            exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status)) //child was killed. Example control + c
            exit_code = 128 + WTERMSIG(status);
    }
    return (exit_code); 
}

int executor(t_cmd *cmd_list, t_env *my_env)
{
    int last_exit_status;

    last_exit_status = 0;
    if (!cmd_list)
        return (0);
    if (cmd_list->next == NULL) 
        last_exit_status = exec_single(cmd_list, my_env);
    return (last_exit_status);
}