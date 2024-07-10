/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 15:25:04 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/27 16:57:37 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Locates the last occurrence of c (converted to a char) in the string pointed
to by s.  The terminating null character is considered to be part of the string;
therefore if c is `\0', the functions locate the terminating `\0'.

Return a pointer to the located character, or NULL if the
     character does not appear in the string.
*/

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*retvalue;
	int			len;

	retvalue = NULL;
	c = (char) c;
	len = ft_strlen(s);
	while (*s != '\0')
		s++;
	if (c == '\0')
		retvalue = s;
	while ((len > 0) && (*s != c))
	{
		s--;
		len--;
	}
	if (*s == c)
		retvalue = s;
	return ((char *)(retvalue));
}
