/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:50:49 by emedina-          #+#    #+#             */
/*   Updated: 2023/04/21 20:50:54 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
	{
		c = c - 32;
	}
	return (c);
}
/*
int main ()
{
	int i;
	char str[] = "salam5anCa7";
	i = 0;
	while (str[i] != '\0')
	{
		str[i] = ft_toupper(str[i]);
		i++;
	}
	printf("%s\n", str);
	return (0);
}
*/