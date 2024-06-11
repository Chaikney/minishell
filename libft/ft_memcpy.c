/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 11:03:52 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/17 19:39:31 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned const char	*str;
	unsigned char		*dest;
	size_t				i;

	str = src;
	dest = dst;
	i = 0;
	if (dst == 0 && src == 0)
		return (0);
	while (i < n)
	{
		dest[i] = str[i];
		i++;
	}
	return (dest);
}
