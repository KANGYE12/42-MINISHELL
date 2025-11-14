

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
static void	ft_lstadd_back(t_token **lst, t_token *new, int has_space)
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
    new->has_space = has_space;
}

//Take into account the tha token is the new message copy from line ultil quote
static t_token *extract_token_from_quote(char *line, int *i)
{
    char *copy;
    int start;
    char quote;

    quote = line[*i];
    start = *i; 
    (*i)++;

    while (line[*i] != '\0' && line[*i] != quote)
        (*i)++;
    if (line[*i] == '\0') 
    {
        printf("minishell: syntax error: unclosed quote\n");
        return (NULL);
    }
    copy = ft_substr(line, start, (*i - start) + 1);
    (*i)++; 
    return (ft_lstnew(copy));
}

//In case > or < are put together
static t_token *extract_token_from_operator(char *line, int *i)
{
    char    *copy;

    if ((line[*i] == '<' && line[*i + 1] == '<') || (line[*i] == '>' && line[*i + 1] == '>')) //Append or herodic found(double operator)
    {
        copy = ft_substr(line, *i, 2); //copy 2 char from start i
        (*i)+=2; //Since it is a double operator form
        return ft_lstnew(copy);
    }
    else
    {
        copy = ft_substr(line, *i, 1);
        (*i)++;
        return ft_lstnew(copy);
    }
}

static t_token *extract_token_from_word(char *line, int *i)
{
    int start;
    char *copy;

    start = *i;
    while(!is_delimiter(line[*i]) && line[*i] != '\0')
        (*i)++;
    copy = ft_substr(line, start, *i - start);
    return ft_lstnew(copy);
}

//Is necessary this kind of implementation in this cases --> "echo>grep c" where a simple split cannot handle it
void    receive_line(char *line_read, t_token **token_list)
{
    int i;
    t_token *new;
    int has_space;

    i = 0;
    has_space = 0;
    while(line_read[i] != '\0')
    {
        if(is_space(line_read[i]))
        {
            has_space = 1;
            i++;
            while(is_space(line_read[i]))
                i++;
            continue;
        }
        else if(line_read[i] == '\'' || line_read[i] == '"') //We put '\ to represent a literal single quote
            new = extract_token_from_quote(line_read, &i);
        else if(is_operator_char(line_read[i])) // < > >> << 
            new = extract_token_from_operator(line_read, &i);
        else //for the actual words that doesnt start from quote
            new = extract_token_from_word(line_read, &i);   
        ft_lstadd_back(token_list, new, has_space);
        has_space = 0;
        new = NULL;              
    }
}
