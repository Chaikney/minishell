/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:17:59 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/17 11:02:12 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Copies n bytes from memory area src to memory area dst.
// If dst and src overlap, behavior is undefined.
// The memcpy() function returns the original value of dst.
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dst;
	s = src;
	if ((d == NULL) && (s == NULL))
		return (dst);
	while (n > 0)
	{
		*d++ = *s++;
		n--;
	}
	return (dst);
}
