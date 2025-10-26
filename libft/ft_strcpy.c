
#include "libft.h"

void ft_strcpy(char *dest, char *source)
{
    int i;

    i = 0;
    while(source[i] != '\0')
    {
        dest[i] = source[i];
        i++;
    }
    dest[i] = '\0';
}