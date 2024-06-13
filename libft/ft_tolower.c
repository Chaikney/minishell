/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:50:42 by emedina-          #+#    #+#             */
/*   Updated: 2023/04/21 21:04:21 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		c = c + 32;
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
		str[i] = ft_tolower(str[i]);
		i++;
	}
	printf("%s\n", str);
	return (0);
}
*/