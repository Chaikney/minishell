/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 15:10:18 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/21 14:48:06 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// return the length of a string, NOT including its NULL byte.
// NOTE this is not protected against non-NT'd strings.
size_t	ft_strlen(const char *str)
{
	size_t	length;

	length = 0;
	if (!str)
		return (length);
	while (str[length] != '\0')
		length++;
	return (length);
}
