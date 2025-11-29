/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 11:41:31 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/24 13:07:31 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int open_input_file(t_cmd *cmd)
{
    int fd = open(cmd->infile, O_RDONLY);
    if (fd < 0)
    {
        perror("open infile");
        return -1;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}

static int open_output_file(t_cmd *cmd)
{
    int fd;
    if (cmd->append)
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open outfile");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return 0;
}

void reset_redirections(void)
{
    dup2(STDIN_FILENO, 0);
    dup2(STDOUT_FILENO, 1);
}

void handle_redirections(t_cmd *cmd)
{
    if (cmd->infile)
    {
        if (open_input_file(cmd) < 0)
            exit(1);
    }

    if (cmd->outfile)
    {
        if (open_output_file(cmd) < 0)
            exit(1);
    }
}