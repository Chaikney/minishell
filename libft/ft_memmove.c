/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 13:46:20 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/03 11:54:41 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Copies len bytes from string src to string dst.
The two strings may overlap;
the copy is always done in a non-destructive manner.

Returns the original value of dst.
[no crash]: your memmove does not segfault when null params is sent
	musl lib has a check that dst and src are not already the same...
	also a check for overlap that is src+len <= dst (and vice versa)
	without that overlap, a straight memcpy is safe.
	Otherwise, you choose forward or back (BSD source the clearest here)
	NB The increment positions are *vital* here, I must understand why.
*/

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*marker;
	const char	*s;

	marker = dst;
	s = src;
	if (marker == s)
		return (dst);
	if ((s + len <= marker) || (marker + len <= s))
		marker = ft_memcpy(marker, s, len);
	else if (s < marker)
	{
		s += len;
		marker += len;
		while (len-- > 0)
			*--marker = *--s;
	}
	else
	{
		while (len-- > 0)
			*marker++ = *s++;
	}
	return (dst);
}
