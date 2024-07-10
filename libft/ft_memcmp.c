/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:41:02 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/04 17:44:58 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Compares byte string s1 against byte string s2.
Both strings are assumed to be n bytes long.

returns zero if the two strings are identical, 
otherwise returns the difference between the first two differing bytes
(treated as unsigned char values, so that `\200' is greater than `\0'
Zero-length strings are always identical.
*/

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	comp1;
	unsigned char	comp2;

	while ((n > 0) && ((*(unsigned char *) s2) == (*(unsigned char *) s1)))
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	comp1 = (*(unsigned char *) s1);
	comp2 = (*(unsigned char *) s2);
	return (comp1 - comp2);
}
