
#include "../includes/minishell.h"
//We are just creating the token whiout linking it with anything
static t_token	*ft_lstnew(char *str)
{
	t_token	*new_list;

	new_list = (t_token *)malloc(sizeof(t_token));
	if (!new_list)
		return (NULL);
	new_list->str = str;
	new_list->next = NULL;
    new_list->prev = NULL;
	return (new_list);
}

//Linking each part of the token in the list of tokens
static void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*temporal;

	if (!lst || !new)
		return ;
	if (*lst == NULL) //If the list starts as NULL. The token is both head and tail of the list
	{
		*lst = new;
		return ;
	}
	temporal = *lst;
	while (temporal->next != NULL)
		temporal = temporal->next;
	temporal->next = new;
    new->prev = temporal;
}

//This function is only used to store the tokens in a linked list that's all. It ignores completly the empty spaces
char *reading_line(t_token **token_list)
{
    char *line;
    char **tokens_split;
    t_token *token;
    size_t i;

    i = 0;
    line = readline("minishell> ");//Display the prompt "minishell> " in the terminal and waits a user to press enter to store it(must be a loop)
    if(line && *line)
        add_history(line); //Add the command line to the history. In this way it using the arrows we can get the previous commands
	else
		return NULL;	
    tokens_split = ft_split(line, ' ');
    while(tokens_split[i] != NULL)
    {
        token = ft_lstnew(tokens_split[i]); //We can also use strdup, but I prefer this way to avoid more allocations. See in the future which one is better
        ft_lstadd_back(token_list, token);
        i++;
    }
	free(tokens_split);
	return line;
}

//Function to classify the different tokens types that can be passed as a argument

void classify_tokens(t_token *token_list)
{
	t_token *current;

	current = token_list;
	while(current != NULL)
	{
		if(ft_strcmp(current->str, "|") == 0)
			current->type = TOKEN_PIPE;
		else if(ft_strcmp(current->str, "<") == 0)
			current->type = TOKEN_REDIR_IN;
		else if(ft_strcmp(current->str, ">") == 0)
			current->type = TOKEN_REDIR_OUT;
		else if(ft_strcmp(current->str, ">>") == 0)
			current->type = TOKEN_APPEND;
		else if(ft_strcmp(current->str, "<<") == 0)
			current->type = TOKEN_HEREDOC;
		else if(ft_strcmp(current->str, "|") == 0)
			current->type = TOKEN_PIPE;
		else //For the remaing parts that are not considered as tokens
			current->type = TOKEN_WORD;		
		current = current->next;					
	}
}