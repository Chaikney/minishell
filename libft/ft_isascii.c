/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 17:22:12 by emedina-          #+#    #+#             */
/*   Updated: 2023/04/21 20:50:13 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
/*
int	main(void)
{
	char	c1;
	char	c2;
	char	c3;
	char	c4;

    c1 = 100;
    c2 = 50;
    c3 = 241;
    c4 = 233;
    if (ft_isascii(c1))
        write(1, "es ASCII\n", 9);
    else
        write(1, "no es ASCII\n", 12);
    if (ft_isascii(c2))
        write(1, "es ASCII\n", 9);
    else
        write(1, "no es ASCII\n", 12);
    if (ft_isascii(c3))
        write(1, "es ASCII\n", 9);
    else
        write(1, "no es ASCII\n", 12);
    if (ft_isascii(c4))
        write(1, "es ASCII\n", 9);
    else
        write(1, "no es ASCII\n", 12);
    return (0);
}
*/