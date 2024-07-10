/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 10:26:39 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/15 17:23:55 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
s1: The string to be trimmed.
set: The reference set of characters to trim.
Return: The trimmed string. NULL if the allocation fails.

Allocates (with malloc(3)) and returns a copy of
’s1’ with the characters specified in ’set’ removed
from the beginning and the end of the string.

FIXED: Error in test 1: ft_strtrim("", ""): not enough memory allocated,
	needed: 1, reserved: 0
Could not find the corresponding allocation or the pointer 0x1053a6de0
NB there's a diff between s1 == NULL and s1[0] == '\0'
 * The imporrtant change seems to have been the addition of s1[start]
 * check in the first while. Need to think on the implications of that
*/

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	start;
	size_t			len;

	if (!s1)
		return (ft_strdup(""));
	if (!set)
		return (ft_strdup(""));
	start = 0;
	len = ft_strlen(s1);
	while ((s1[start]) && (ft_strchr(set, s1[start]) != NULL))
		start++;
	if (s1[start] == '\0')
		return (ft_strdup(""));
	while (ft_strrchr(set, s1[len]) != NULL)
		len--;
	len = (len - start) + 1;
	return (ft_substr(s1, start, len));
}
