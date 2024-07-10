/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 10:36:23 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/16 10:36:44 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Print the given string s to the file descriptor fd
// Return number of chars printed.
// calling function has to free (or not) the string passed in.
int	pf_putstr_fd(char *s, int fd)
{
	int	haveput;

	haveput = 0;
	if ((fd >= 0) && (fd <= 1023))
	{
		while (*s != '\0')
		{
			haveput += pf_putchar_fd(*s, fd);
			s++;
		}
	}
	return (haveput);
}
