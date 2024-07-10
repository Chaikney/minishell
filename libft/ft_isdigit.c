/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:31:36 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 14:32:32 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// returns zero if false and non-zero if true. The value of the argument must
// be representable as an unsigned char or the value of EOF.
#include "libft.h"

int	ft_isdigit(int c)
{
	int	retcode;

	retcode = 0;
	if (!c)
		return (retcode);
	if ((ft_isascii(c) == 1) && (c >= 48) && (c <= 57))
		retcode = 1;
	return (retcode);
}
