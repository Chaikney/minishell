/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_digits.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:01:05 by chaikney          #+#    #+#             */
/*   Updated: 2023/08/23 16:01:13 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Returns the number of base digits in the integer n
// +/- sign is not included.
// Return 0 if there's no (usable) base
int	ft_get_digits(int n, int base)
{
	if ((!base) || (base <= 0))
		return (0);
	if (n < 0)
		return (ft_get_digits(n * -1, base));
	if (n < base)
		return (1);
	return (1 + ft_get_digits(n / base, base));
}
