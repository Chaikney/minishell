/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:49:35 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/03 15:38:22 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
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
	int i = 0;
	int c = 60;
	while(i++ < 10)
	{
		printf("%d.El return de la ft es %d\n", i, ft_isalpha(c));
		c++;
	} */
	/* 
	if(ft_isalpha('5'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	}
	if(ft_isalpha('g'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	}
	if(ft_isalpha('G'))
	{
		printf("%s\n", "Es alfanumerico");
	}
	else
	{
		printf("%s\n", "No es alfanumerico");
	} */
	//return(0);
//}
