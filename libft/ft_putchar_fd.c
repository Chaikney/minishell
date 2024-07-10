/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:23:24 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/17 10:42:03 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Output character c to specified file descriptor fd.
// NOTE Maximum fd is hard-coded as 1024. This could be improved.
void	ft_putchar_fd(char c, int fd)
{
	if ((fd >= 0) && (fd <= 1024))
	{
		write(fd, &c, 1);
	}
}
