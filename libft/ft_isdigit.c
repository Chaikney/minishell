/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 14:05:58 by emedina-          #+#    #+#             */
/*   Updated: 2023/04/21 20:50:20 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
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
	if(ft_isdigit('5'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	}
	if(ft_isdigit('g'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	}
}
*/