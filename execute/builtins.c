/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isrguerr <isrguerr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:31:06 by iisraa11          #+#    #+#             */
/*   Updated: 2025/12/19 12:51:00 by isrguerr         ###   ########.fr       */
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

int check_echo_n(char *str)
{
    int i;
    size_t counter;

    if(str[0] == '-' && str[1] == 'n')
    {
        i = 1;
        counter = 0;
        while(str[i] && str[i] == 'n')
        {
            i++;
            counter++;
        }
        if(counter == ft_strlen(str) - 1)    
            return 1;
    }    
    return 0;
}

int builtin_echo(char **argv)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (argv[1] && check_echo_n(argv[1]) == 1)
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