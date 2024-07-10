/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:32:14 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/24 10:38:04 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Returns 1 if character c is alphapbetic.
// Returns 0 if character is not, or no character passed.
int	ft_isalpha(int c)
{
	int	retcode;

	if (!c)
		return (0);
	if (ft_isascii(c) == 0)
		retcode = 0;
	else if ((c >= 'a') && (c <= 'z'))
		retcode = 1;
	else if ((c >= 'A') && (c <= 'Z'))
		retcode = 1;
	else
		retcode = 0;
	return (retcode);
}
