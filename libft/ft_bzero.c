/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:11:19 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 12:57:35 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Overwrites an area with NULL
// Starts at s and runs for n characters.
void	ft_bzero(void *s, size_t n)
{
	unsigned char	*str;

	if (!n)
		return ;
	str = (unsigned char *) s;
	while (n > 0)
	{
		*str = '\0';
		str++;
		n--;
	}
	return ;
}
