/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 11:34:25 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/02 11:55:04 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
s: The string on which to iterate.
f: The function to apply to each character.

Applies the function ’f’ on each character of the string passed as argument,
passing its index as first argument.
Each character is passed by address to ’f’ to be modified if necessary.
*/

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}
