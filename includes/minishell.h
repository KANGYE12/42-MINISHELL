#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
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

//A simple enum to differentiate each of the token types
typedef enum e_token_type {
    TOKEN_WORD,       // e.g. echo, hello, world
    TOKEN_PIPE,       // |
    TOKEN_REDIR_IN,   // <
    TOKEN_REDIR_OUT,  // >
    TOKEN_APPEND,     // >>
    TOKEN_HEREDOC,    // <<
    TOKEN_EOF,        // end of input (optional)
} t_token_type;


//Each argument passed to the executable is considered a token which will be stored in this struct
typedef struct s_token {
    char    *str;
    t_token_type     type;
    struct s_token *prev;
    struct s_token *next;
}   t_token;

void reading_line(t_token **token_list);

#endif

