/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 10:16:53 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/16 16:45:41 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Parameters n: the integer to convert.
Return value The string representing the integer.
NULL if the allocation fails.
External functs. malloc
Description Allocates (with malloc(3)) and returns a string
representing the integer received as an argument.
Negative numbers must be handled
 * NB -- this can be done much more neatly with strjoin and recursion :(
 * MIN_INT = *-1, but if you do that for a signed int, you overflow MAX
 * ZERO, the only prob is getting digit number.
 * MAX_INT:	2147483647	(needs 11 bytes)
 * MIN_INT:	-2147483648	(needs 12 bytes)
*/

#include "libft.h"

static char	digit_as_char(int i)
{
	char	c;

	c = '\0';
	if ((i >= 0) && (i <= 9))
		c = (i + 48);
	return (c);
}

static char	*ft_makestring(int n, int nd, int sign, int size)
{
	int		digit;
	char	*retstr;
	int		i;

	digit = 0;
	i = 0;
	retstr = (char *)ft_calloc (size, sizeof(char));
	if (retstr == NULL)
		return (NULL);
	if (sign == -1)
	{
		retstr[i] = '-';
		i++;
	}
	while ((nd > 0) && (i < size))
	{
		digit = n / ft_powerof10(nd - 1);
		n = n % ft_powerof10(nd - 1);
		retstr[i] = digit_as_char(digit);
		i++;
		nd--;
	}
	retstr[i] = '\0';
	return (retstr);
}

char	*ft_itoa(int n)
{
	int		sign;
	int		numdigits;
	int		array_size;

	numdigits = 0;
	sign = 1;
	if (n < 0)
		sign = -1;
	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	else if (n < 0)
		n = (n * -1);
	numdigits = ft_get_digits(n, 10);
	array_size = (numdigits + 1);
	if (sign == -1)
		array_size++;
	return (ft_makestring(n, numdigits, sign, array_size));
}
