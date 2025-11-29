/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:31:06 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/29 12:34:11 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_pwd(void)
{
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)))
    {
        write(1, cwd, ft_strlen(cwd));
        write(1, "\n", 1);
    }
    return (0);
}

int builtin_echo(char **argv)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }

    while (argv[i])
    {
        write(1, argv[i], ft_strlen(argv[i]));
        if (argv[i + 1])
            write(1, " ", 1);
        i++;
    }

    if (newline)
        write(1, "\n", 1);

    return (0);
}

int builtin_env(t_env *env_list)
{
    t_env *tmp;
    
    tmp = env_list;
    while (tmp)
    {
        write(1, tmp->key, ft_strlen(tmp->key));
        write(1, "=", 1);
        write(1, tmp->value, ft_strlen(tmp->value));
        write(1, "\n", 1);
        tmp = tmp->next;
    }
    return (0);
}


int builtin_exit(char **argv)
{
    if (!is_digit_str(argv[1]) && argv[1])
    {
        printf("minishell: exit: %s: numeric argument required\n", argv[1]);
        exit(255);
    }
    
    int status;

    status = 0;

    if (argv[1])
        status = ft_atoi(argv[1]);

    printf("exit\n");
    exit(status);
}

int builtin_cd(char **argv)
{
    if (!argv[1])
    {
        printf("minishell: cd: missing argument\n");
        return 1;
    }

    if (chdir(argv[1]) != 0) // cambia el directorio
    {
        perror("minishell: cd");
        return 1;
    }

    return 0;
}
