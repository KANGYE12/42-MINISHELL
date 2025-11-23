
#include "../includes/minishell.h"

//This function is only used to store the tokens in a linked list that's all. It ignores completly the empty spaces
char *reading_line(t_token **token_list)
{
    char *line;

    line = readline("minishell> ");//Display the prompt "minishell> " in the terminal and waits a user to press enter to store it(must be a loop)
	if(!line)
		return NULL;
	if(line[0] == '\0')
		return line;
	add_history(line);		
    receive_line(line, token_list);
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
