/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:45:14 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 14:02:36 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Tests for an ASCII character, i.e. between 0 and octal 0177 inclusive.
// Returns 1 if true, 0 if false.
int	ft_isascii(int c)
{
	int	retcode;

	retcode = 0;
	if ((c >= 0) && (c <= 127))
		retcode = 1;
	return (retcode);
}
