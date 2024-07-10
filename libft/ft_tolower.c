/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:57:48 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 10:12:00 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Return lowercase equivalent of char c
// NOTE ascii only
int	ft_tolower(int c)
{
	if (ft_isalpha(c) == 0)
		return (c);
	else if ((c >= 65) && (c <= 90))
		c = (c + 32);
	return (c);
}
