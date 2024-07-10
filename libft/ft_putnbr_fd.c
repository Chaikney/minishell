/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:45:16 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/16 17:13:55 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * DONE: reduce scope, use other functions.
 * 	...ike this should just turn the number into a string for ft_putstr_fd
 * 	...implies use of ft_itoa, no??	EXCEPT that raised leaks, so moved the 
 * 	logic of it in here.
 * STEPS
 * * deal with INT_MIN
 * * deaal with negative numbers (print - and go absolute)
 * * next time around pass in a div by 10
 * * * (when you return from that, print the mod (i.e. lower digit)
 * * * (rememmeber that we're unwinding this as we recurse!)
 * * once we've reached a digit between 0 and 9 we can just print it.
 */

#include "libft.h"

void	ft_putnbr_fd(int nbr, int fd)
{
	if (nbr == INT_MIN)
		ft_putstr_fd("-2147483648", fd);
	else if (nbr < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd((nbr * -1), fd);
	}
	else if (nbr >= 10)
	{
		ft_putnbr_fd(nbr / 10, fd);
		ft_putchar_fd((nbr % 10) + 48, fd);
	}
	else
		ft_putchar_fd(nbr + 48, fd);
}
