/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 12:12:12 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 10:38:40 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Return 1 if c is a printable character
// Return 0 otherwise (i.e. NULL or control char)
int	ft_isprint(int c)
{
	int	retcode;

	if (!c)
		return (0);
	if ((c >= 32) && (c <= 126))
		retcode = 1;
	else
		retcode = 0;
	return (retcode);
}
