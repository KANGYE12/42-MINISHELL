
#include "../includes/minishell.h"

/*
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

//Not finished, must to add some more details
t_cmd *parse_tokens_to_cmds(t_token *token_list, t_cmd *cmd_list)
{
	classify_tokens(token_list);
	t_cmd *new_cmd;
	t_token *current;
	int i;

	current = token_list;
	new_cmd = ft_lstnew();
	i = 0;
	while(current != NULL)
    {
        if(current->type == TOKEN_WORD)
            new_cmd->argv[i++] = current->str;
        else if(current->type == TOKEN_PIPE) //It means that the current command ends here and we need to prepare for then next one
        {
            new_cmd->argv[i] == NULL;
            new_cmd->next = ft_lstnew();
            new_cmd = new_cmd->next;
        }
        else if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND) //Write in a new output file or in a existing one
        {
            new_cmd->outfile = current->next->str;
            new_cmd->append = (current->type == TOKEN_APPEND) ? 1 : 0;
            current = current->next; 
        }
        else if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
        {
            new_cmd->infile = current->next->str;
            current = current->next;
        }
        current = current->next;
    }
    ft_lstadd_back(cmd_list, new_cmd);
}
*/