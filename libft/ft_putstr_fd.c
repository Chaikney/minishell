/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:47:38 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/21 13:26:30 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// NOTE the fd check here is inconsistent with that in putchar
// Output string s to file descriptor fd.
void	ft_putstr_fd(char *s, int fd)
{
	if ((fd >= 0) && (fd <= 1023))
	{
		while (*s != '\0')
		{
			ft_putchar_fd(*s, fd);
			s++;
		}
	}
}
