/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putsigned_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 10:36:54 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/16 10:37:00 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Takes signed int, puts a "-" if needed then calls pf_put_unsigned_fd.
int	pf_put_signed_fd(int nbr, int fd)
{
	int	put;

	put = 0;
	if (nbr < 0)
	{
		put += pf_putchar_fd('-', fd);
		nbr *= -1;
	}
	put += pf_put_unsigned_fd(nbr, fd);
	return (put);
}
