/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putunsigned_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 10:35:45 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/16 10:35:52 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// Takes unsigned int and uses recursive division to find the digits.
// Uses a string lookup with putchar to print the output to file descriptor fd.
int	pf_put_unsigned_fd(unsigned int nbr, int fd)
{
	int	put;

	put = 1;
	if (nbr >= 10)
		put = put + pf_put_unsigned_fd((nbr / 10), fd);
	pf_putchar_fd("0123456789"[nbr % 10], fd);
	return (put);
}
