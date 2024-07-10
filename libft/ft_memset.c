/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:53:23 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 14:50:32 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Copies char c to position dest, len times
void	*ft_memset(void *dest, int c, size_t len)
{
	unsigned char	*s;

	s = (unsigned char *) dest;
	while (len > 0)
	{
		*s = c;
		s++;
		len--;
	}
	return (dest);
}
