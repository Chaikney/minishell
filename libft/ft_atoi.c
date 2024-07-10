/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:30:06 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/25 14:03:13 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* The string may begin with an arbitrary amount of white space
   (as determined by isspace(3)) followed by
   a single optional `+' or `-' sign.
 */

static int	ft_array_to_int(int *arr, int numdigits)
{
	int	total;
	int	i;

	i = 0;
	total = 0;
	numdigits--;
	while (i <= numdigits)
	{
		total += ft_powerof10 (i) * (arr[(numdigits - i)]);
		i++;
	}
	return (total);
}

int	ft_atoi(const char *str)
{
	int	signcount;
	int	nodigits;
	int	digits[10];

	nodigits = 0;
	signcount = 1;
	while (*str == 32 || (*str <= 13 && *str >= 9))
		str++;
	if ((*str == '-') || (*str == '+') || (*str == '0'))
	{
		if (*str == '-')
			signcount = -1;
		str++;
		while (*str == '0')
			str++;
	}
	while (ft_isdigit(*str) == 1)
	{
		digits[nodigits] = (*str - 48);
		nodigits++;
		str++;
	}
	return (ft_array_to_int(digits, nodigits) * signcount);
}
