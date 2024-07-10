/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:53:33 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 10:11:37 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Return the equivalent uppercase of char c
// NOTE ascii only
int	ft_toupper(int c)
{
	if (ft_isalpha(c) == 0)
		return (c);
	else if ((c >= 97) && (c <= 122))
		c = (c - 32);
	return (c);
}
