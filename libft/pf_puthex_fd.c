/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_puthex_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 10:37:12 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/16 10:37:36 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Direct printing of hexadecimal using either big or little letters.
// nbr - what to print
// f - format code as per printf.
// fd - file descriptor to send output to
// Uses unsigned long so that pointers are not cut short
int	pf_puthex_fd(unsigned long nbr, char f, int fd)
{
	int	put;

	put = 1;
	if ((f == 'x') || (f == 'p'))
	{
		if (nbr >= 16)
			put = put + pf_puthex_fd((nbr / 16), f, fd);
		pf_putchar_fd("0123456789abcdef"[nbr % 16], fd);
	}
	else if (f == 'X')
	{
		if (nbr >= 16)
			put = put + pf_puthex_fd((nbr / 16), f, fd);
		pf_putchar_fd("0123456789ABCDEF"[nbr % 16], fd);
	}
	return (put);
}
