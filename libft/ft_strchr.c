/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 21:23:59 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/17 19:43:13 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s != (char)c)
		{
			s++;
		}
		else
		{
			return ((char *)s);
		}
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
