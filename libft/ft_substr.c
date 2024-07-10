/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 11:10:14 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/16 14:28:29 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
s: The string from which to create the substring.
start: The start index of the substring in the string ’s’.
len: The maximum length of the substring.
Return: The substring.  or NULL if the allocation fails.

Allocates (with malloc(3)) and returns a substring from the string ’s’.
The substring begins at index ’start’ and is of maximum size ’len’.
*/

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*sub;
	size_t		howbig;
	const char	*startpoint;

	howbig = 0;
	if (start < ft_strlen(s))
	{
		startpoint = &s[start];
		if ((ft_strlen(s) - start) < len)
			howbig = (ft_strlen(s) - start);
		else
			howbig = (len);
	}
	else
		startpoint = &s[0];
	sub = malloc((howbig + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	if (howbig != 0)
		ft_strlcpy(sub, startpoint, howbig + 1);
	else
		sub[0] = '\0';
	return (sub);
}
