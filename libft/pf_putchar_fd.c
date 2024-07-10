/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 16:59:59 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/10 17:00:09 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Print char c to file descriptor fd.
// Return the number of chars sucessfully printed.
int	pf_putchar_fd(char c, int fd)
{
	int	cput;

	cput = 0;
	if ((fd >= 0) && (fd <= RLIMIT_NOFILE))
		cput = write(fd, &c, 1);
	if (cput == -1)
		cput = 0;
	return (cput);
}
