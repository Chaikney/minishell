/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 18:46:04 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/17 10:43:51 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
Allocates sufficient memory for a copy of the string s1, does the copy,
and returns a pointer to it.
The pointer may subsequently be used as an argument to the function free(3).
If insufficient memory is available, NULL is returned and errno is set to ENOMEM.
 * (This variable and associated constants require errno.h)
*/

#include "libft.h"
#include <errno.h>

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*newstring;

	len = ft_strlen(s1) + 1;
	newstring = (char *)malloc(len * sizeof(char));
	if (newstring == NULL)
	{
		errno = ENOMEM;
		return (newstring);
	}
	ft_strlcpy(newstring, s1, len);
	return (newstring);
}
