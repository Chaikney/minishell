/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:49:02 by emedina-          #+#    #+#             */
/*   Updated: 2023/04/21 20:50:25 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
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
	if(ft_isprint('5'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	}
	if(ft_isprint('g'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	}
	if(ft_isprint('ñ'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	}
}
*/