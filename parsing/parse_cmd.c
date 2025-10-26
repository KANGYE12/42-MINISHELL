
#include "../includes/minishell.h"


static t_cmd	*ft_lstnew()
{
	t_cmd	*new_list;

	new_list = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_list)
		return (NULL);
	new_list->argv = NULL;
	new_list->infile = NULL;
    new_list->outfile = NULL;
    new_list->next = NULL;
    new_list->append = 0; // 1 if it is necesary append it
	return (new_list);
}

static void	ft_lstadd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*temporal;

	if (!lst || !new)
		return ;
	if (*lst == NULL) 
	{
		*lst = new;
		return ;
	}
	temporal = *lst;
	while (temporal->next != NULL)
		temporal = temporal->next;
	temporal->next = new;
}

static void add_to_argv(t_cmd *new_cmd, char *str, int *args)
{
	char **temp_argv;
	char *temp_str;
	size_t len;
	
	temp_argv = realloc(new_cmd->argv, sizeof(char *)*(*args + 2)); // + 2 since we need to realloc the new string and the NULL args
	if(!temp_argv)
	{
		perror("ERROR IN REALLOC");
		return ;
	}
	len = ft_strlen(str);
	temp_str = malloc(sizeof(char)*(len + 1));
	if(!temp_str)
	{
		perror("ERROR IN MALLOC ADD_TO_ARGV");
		free(temp_str);
		return ;
	}
	new_cmd->argv = temp_argv;
	new_cmd->argv[*args] = temp_str;
	ft_strcpy(new_cmd->argv[*args], str);
	(*args)++;
	new_cmd->argv[*args] = NULL;
}

static void handle_infile(t_cmd *new_cmd, t_token *current_token)
{
	//since we already checked that the next token is a word, then is not necesary check it again
	//(I should create more checks in a close future)
	char *temp_infile;
	size_t len;

	if(new_cmd->infile) //there is already data in the infile we need to overwrite it using free first
		free(new_cmd->infile);
	len = ft_strlen(current_token->next->str);
	temp_infile = malloc(sizeof(char)*(len + 1));
	if(!temp_infile)
	{
		perror("MALLOC OF HANDLE_INFILE IS NULL");
		return ;
	}
	new_cmd->infile = temp_infile;
	ft_strcpy(new_cmd->infile, current_token->next->str);
}

static void handle_outfile(t_cmd *new_cmd, t_token *current_token, int append)
{
	char *temp_outfile;
	size_t len;

	if(new_cmd->outfile) //there is already data in the infile we need to overwrite it using free first
		free(new_cmd->outfile);
	len = ft_strlen(current_token->next->str);
	temp_outfile = malloc(sizeof(char)*(len + 1));
	if(!temp_outfile)
	{
		perror("MALLOC OF HANDLE_OUTFILE IS NULL");
		return ;
	}
	new_cmd->outfile = temp_outfile;
	ft_strcpy(new_cmd->outfile, current_token->next->str);
	new_cmd->append = append;
}	

//Not finished, must to add some more details(still necesary HEREDOC)
t_cmd *parse_tokens_to_cmds(t_token *token_list)
{
    t_cmd *cmd_list = NULL;
    t_cmd *new_cmd = ft_lstnew();
    t_token *current = token_list;
    int argc = 0;

	if(!new_cmd)
		return NULL;
    while (current)
    {
        if (current->type == TOKEN_WORD)
            add_to_argv(new_cmd, current->str, &argc);
        else if (current->type == TOKEN_REDIR_IN)
		{
			handle_infile(new_cmd, current);
			current = current->next; //we want to skip the next token since it is already used
		}
        else if (current->type == TOKEN_REDIR_OUT)
		{
			handle_outfile(new_cmd, current, 0);
			current = current->next;
		}
        else if (current->type == TOKEN_APPEND)
		{
			handle_outfile(new_cmd, current, 1);
			current = current->next;
		}
        else if (current->type == TOKEN_PIPE)
        {
            ft_lstadd_back(&cmd_list, new_cmd);
            new_cmd = ft_lstnew();
            argc = 0;
        }
        current = current->next;
    }
	if(new_cmd)
	{
		ft_lstadd_back(&cmd_list, new_cmd);
    	return cmd_list;
	}
	return NULL;
}

