/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strchr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 14:25:40 by chaikney          #+#    #+#             */
/*   Updated: 2023/04/27 16:37:48 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Return a pointer to the first occurence of c in string s
// or NULL if the character does not appear in the string.
// NOTE The terminating null character is considered to be part of the string;
// therefore if c is `\0', the functions locate the terminating `\0'.
char	*ft_strchr(const char *s, int c)
{
	const char	*retvalue;

	c = (char) c;
	retvalue = NULL;
	while ((*s != '\0') && (*s != c))
	{
		s++;
	}
	if (*s == c)
		retvalue = s;
	return ((char *)(retvalue));
}
