
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

//This function is only used to store the tokens in a linked list that's all
void reading_line(t_token **token_list)
{
    char *line;
    char **tokens_split;
    t_token *token;
    size_t i;

    i = 0;
    line = readline("minishell> ");//Display the prompt "minishell> " in the terminal and waits a user to press enter to store it(must be a loop)
    if(line && *line)
        add_history(line); //Add the command line to the history. In this way it using the arrows we can get the previous commands
    tokens_split = ft_split(line, ' ');
    while(tokens_split[i] != NULL)
    {
        token = ft_lstnew(tokens_split[i]);
        ft_lstadd_back(token_list, token);
        i++;
    }
}
