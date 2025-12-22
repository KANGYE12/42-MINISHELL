/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 16:29:07 by iisraa11          #+#    #+#             */
/*   Updated: 2025/12/22 16:33:25 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

# define SUCCESS 0
# define ERROR 1
# define SYNTAX_ERROR 1
# define UNKNOWN_COMMAND 1

extern int			g_signal_status;

// A simple enum to differentiate each of the token types
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
}					t_token_type;

// Each argument passed to the executable is considered
// a token which will be stored in this struct
typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*prev;
	struct s_token	*next;

	int				has_space;
}					t_token;

// Change the token list into a cmd command list
typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	int				heredoc_fd;
	char			*outfile;
	int				append;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_heredoc
{
	int				fd;
	char			*delimiter;
	int				expansion;
	t_env			**env;
	int				last_exit_status;
}					t_heredoc;

// statest for a variable expansion
typedef struct s_exp
{
	int				i;
	int				start;
	int				quotes[2];
	t_env			**env;
	int				last_exit_status;
}					t_exp;

// Enviroment code
char				*get_env_value(t_env **env, char *key);
void				free_env(t_env **env);
t_env				*init_env_list(char **envp);
void				replace_tokens_variables(t_token *token_list, t_env **env,
						int last_exit_status);
char				**env_list_to_array(t_env *env_list);
char				*expand_variables(char *str, t_env **env,
						int last_exit_status);

// tokens
char				*reading_line(t_token **token_list);
void				classify_tokens(t_token *token_list);
int					is_operator_char(char c);
int					is_space(char c);
void				receive_line(char *line_read, t_token **token_list);
int					check_syntax_tokens(t_token *token_list);
char				**resize_argv(char **old, int size);
void				read_heredoc_input(t_heredoc *hdoc);
t_cmd				*ft_lstnew_parse(void);
void				ft_lstadd_back_parse(t_cmd **lst, t_cmd *new);

void				handle_redirections_and_heredoc(t_cmd *cmd,
						t_token **current, t_env **env, int last_exit_status);

void				handle_pipe(t_cmd **cmd_list, t_cmd **new_cmd, int *argc);

void				handle_heredoc(t_cmd *cmd, t_heredoc *hdoc);
int					handle_infile(t_cmd *cmd, t_token *token);
int					handle_outfile(t_cmd *cmd, t_token *token, int append);

char				*ft_strjoin_free_s1(char *s1, char *s2);
char				*get_var_name(const char *str, int *len);
char				*dup_env_or_empty(const char *s);

t_token				*extract_token_from_quote(char *line, int *i);
t_token				*extract_token_from_operator(char *line, int *i);
t_token				*extract_token_from_word(char *line, int *i);
void				ft_lstadd_back_token(t_token **lst, t_token *new,
						int has_space);
t_token				*ft_lstnew_token(char *str);

// special token cases
void				merge_adjacent_words(t_token **token_list);

// utils
void				error_message(char *message, t_token *token_list);
void				free_token_list(t_token **token_list);
void				free_cmd_list(t_cmd **cmd_list);
void				free_double_ptr(char **ptr);
int					is_delimiter(char c);
int					is_var_char(char c);
char				*clean_quotes_from_str(char *delimiter);
void				handle_sigint(int sig);
void				remove_quotes_from_list(t_token *token_list);
int					is_digit_str(char *str);

// cmd command
t_cmd				*parse_tokens_to_cmds(t_token *token_list, t_env **env,
						int last_exit_status);

// execute
int					executor(t_cmd *cmd_list, t_env **my_env);

// path
char				*find_cmd_path(char *cmd, t_env *my_env);

// builtins
int					exec_builtin(t_cmd *cmd, t_env **my_env, int in_parent);
int					builtin_cd(char **argv);
int					builtin_export(char **argv, t_env **env_list);
int					builtin_unset(char **argv, t_env **env_list);
int					builtin_env(t_env *env_list);
int					builtin_exit(char **argv);
int					builtin_echo(char **argv);
int					builtin_pwd(void);
int					is_builtin(char *cmd);

// redirections
void				handle_redirections(t_cmd *cmd);

#endif
