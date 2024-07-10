/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reverse_string.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 14:59:51 by chaikney          #+#    #+#             */
/*   Updated: 2023/08/24 14:59:59 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// A reverse string function. Takes string and length,
// returns equal-length string backwards.
char	*ft_reverse_string(char *str, size_t len)
{
	char	*rts;
	int		i;

	if ((!str) || (!len) || (len == 0))
		return (NULL);
	rts = malloc ((len + 1) * sizeof(char));
	if (!rts)
		return (NULL);
	i = 0;
	while (len > 0)
	{
		rts[i] = str[len - 1];
		len--;
		i++;
	}
	rts[i] = '\0';
	return (rts);
}
