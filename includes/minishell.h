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


# define SUCCESS 0
# define ERROR 1
# define SYNTAX_ERROR 1
# define UNKNOWN_COMMAND 1

//A simple enum to differentiate each of the token types
typedef enum e_token_type {
    TOKEN_WORD,       //e.g. echo, hello, world --> For the execution part are you hearing me Israel
    TOKEN_PIPE,       //|
    TOKEN_REDIR_IN,   //<
    TOKEN_REDIR_OUT,  //>
    TOKEN_APPEND,     //>>
    TOKEN_HEREDOC,    //<<
} t_token_type;


//Each argument passed to the executable is considered a token which will be stored in this struct
typedef struct s_token {
    char    *str;
    t_token_type     type;
    struct s_token *prev;
    struct s_token *next;
}   t_token;

//Change the token list into a cmd command list
typedef struct s_cmd {
    char **argv;  //Array of strings
    char *infile; // < or << --> Read only
    //It writes directly in the new part
    char *outfile; // >
    int append; //>>
    struct s_cmd *next; // For pipes
} t_cmd;


char *reading_line(t_token **token_list);
void classify_tokens(t_token *token_list);
void free_token_list(t_token *token_list);
void free_split_array(char **arr);
int syntax_check(t_token *token_list);
t_cmd *parse_tokens_to_cmds(t_token *token_list, t_cmd *cmd_list);
int is_operator_char(char c);
int is_space(char c);
void    receive_line(char *line_read, t_token **token_list);


#endif

