#include "../includes/minishell.h"

int	check_syntax_tokens(t_token *token_list) //check if the tokens we passed as an argument is right
{
	t_token *current;

	current = token_list;
	if(token_list == NULL)
		return SUCCESS; //The user only has pressed enter
	else if(current->type == TOKEN_PIPE)
		return SYNTAX_ERROR;

	while(current->next != NULL)
	{
		if(current->type != TOKEN_WORD && current->next->type != TOKEN_WORD)
			return SYNTAX_ERROR;
		current = current->next;	
	}
	if(current->type != TOKEN_WORD) //The last token must be a word type
		return SYNTAX_ERROR;
	return SUCCESS;
}