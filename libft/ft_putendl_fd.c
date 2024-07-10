/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 13:55:24 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/21 14:02:50 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Prints a string followed by a newline
void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}
