/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 15:36:46 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/12 11:51:25 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
copies up to dstsize - 1 characters from the string src to dst,
NUL-terminating the result if dstsize is not 0.
The strlcpy() and strlcat() functions return the total length of the string 
they tried to create.  For strlcpy() that means the length of src.
 * FIXED:	writing the final \0 (l38?) causes heap buffer overflows elsewhere.
 * FIXED: doesn't write the first char? No, it overwrote it.
 * 	added a break out of the loop as soon as we have copied a null from src
*/

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t		srclen;
	size_t		charsleft;
	char		*d;
	const char	*s;

	d = dst;
	s = src;
	charsleft = dstsize;
	srclen = ft_strlen(src);
	if ((charsleft != 0))
	{
		while (--charsleft != 0)
		{
			*d = *s;
			if (*s == '\0')
				break ;
			s++;
			d++;
		}
	}
	if ((charsleft == 0) && (dstsize != 0))
		*d = '\0';
	return (srclen);
}
