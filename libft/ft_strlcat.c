/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 15:49:40 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/12 11:06:43 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* strlcat() appends string src to the end of dst.
* It will append at most dstsize - strlen(dst) - 1 characters.
* It will then NUL-terminate, unless dstsize is 0 or 
	the original dst string was longer than dstsize
	(in practice this should not happen as it means that either 
	dstsize is incorrect or that dst is not a proper string).

* Take the full size of the destination buffer and guarantee NUL-termination
if there is room.  Note that room for the NUL should be included in dstsize.
If the src and dst strings overlap, the behavior is undefined.

* Return the total length of the string they tried to create.
For strlcat() that means the initial length of dst plus the length of src.
	origsize = (d - dst);
	// need to ID what this does in BSD because why not strlen call? Speed?
	// more show-offy pointer arithmetic
*/

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t		origsize;
	size_t		lefttoadd;
	char		*d;
	const char	*s;

	d = dst;
	s = src;
	lefttoadd = dstsize;
	while ((*d != '\0') && (lefttoadd-- != 0))
		d++;
	origsize = (d - dst);
	lefttoadd = (dstsize - origsize);
	if (lefttoadd == 0)
		return (origsize + ft_strlen(src));
	while (*s != '\0')
	{
		if (lefttoadd != 1)
		{
			*d++ = *s;
			lefttoadd--;
		}
		s++;
	}
	*d = '\0';
	return (origsize + ft_strlen(src));
}
