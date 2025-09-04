#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <termios.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# define _ECHO 1
# define _CD 2
# define _PWD 3
# define _EXPORT 4
# define _UNSET 5
# define _ENV 6
# define _EXIT 7

# define SUCCESS 0
# define ERROR 1
# define SYNTAX_ERROR 258
# define UNKNOWN_COMMAND 127

typedef struct job_t{
    int id;
    char *command;
    pid_t pid;
    int status;
} job_t;


#endif