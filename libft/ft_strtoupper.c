/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoupper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 15:00:09 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/16 10:38:38 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Alters the string passed to it, making chars uppercase.

#include "libft.h"

void	ft_strtoupper(char *str)
{
	while (*str != '\0')
	{
		if ((*str >= 97) && (*str <= 122))
			*str = (*str - 32);
		str++;
	}
}
