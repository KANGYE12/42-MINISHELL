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

    int has_space;
}   t_token;

//Change the token list into a cmd command list
typedef struct s_cmd {
    char **argv;  //Array of strings
    char *infile; // <
    int heredoc_fd; //<<

    //It writes directly in the new part
    char *outfile; // >
    int append; //>> only if it is 1

    //next structure
    struct s_cmd *next; // For pipes
} t_cmd;

typedef struct s_env
{
    char            *key; //USER
    char            *value; //VALUE OF USER
    struct s_env    *next;
} t_env;


//Enviroment code
char    *get_env_value(t_env **env, char *key);
void    free_env(t_env **env);
t_env *init_env_list(char **envp);
void replace_tokens_variables(t_token *token_list, t_env **env, int last_exit_status);
char **env_list_to_array(t_env *env_list);
char *expand_variables(char *str, t_env **env, int last_exit_status);


//tokens
char *reading_line(t_token **token_list);
void classify_tokens(t_token *token_list);
int is_operator_char(char c);
int is_space(char c);
void    receive_line(char *line_read, t_token **token_list);
int	check_syntax_tokens(t_token *token_list);

//special token cases
void merge_adjacent_words(t_token **token_list);

//utils
void	error_message(char *message, t_token *token_list);
void free_token_list(t_token **token_list);
void free_cmd_list(t_cmd **cmd_list);
void free_double_ptr(char **ptr);
int is_delimiter(char c);
int is_var_char(char c);
char *clean_quotes_from_str(char *delimiter);

void remove_quotes_from_list(t_token *token_list);


//cmd command
t_cmd *parse_tokens_to_cmds(t_token *token_list, t_env **env, int last_exit_status);

//execute
int executor(t_cmd *cmd_list, t_env *my_env);

//path
char *find_cmd_path(char *cmd, t_env *my_env);


#endif

