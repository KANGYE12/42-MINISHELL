/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iisraa11 <iisraa11@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 18:35:14 by iisraa11          #+#    #+#             */
/*   Updated: 2025/11/30 18:39:31 by iisraa11         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	size_t	j;
	char	*dup;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
		i++;
	dup = (char *)malloc(i + 1);
	if (!dup)
		return (NULL);
	j = -1;
	while (++j < i)
		dup[j] = s[j];
	dup[i] = '\0';
	return (dup);
}
