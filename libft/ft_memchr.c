/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:07:41 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/11 19:50:31 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Locates first occurrence of c (converted to an unsigned char) in string s.
// Returns a pointer to the byte located,
// or NULL if no such byte exists within n bytes.
void	*ft_memchr(const void *s, int c, size_t n)
{
	void	*here;
	char	*str;
	size_t	i;

	i = 0;
	here = NULL;
	str = (char *) s;
	if ((!n) || (n == 0))
		return (here);
	while (i < n)
	{
		if ((unsigned char)*str == (unsigned char)c)
			return ((char *) str);
		str++;
		i++;
	}
	if ((c == '\0') && (*str == '\0'))
		here = (char *) str;
	return (here);
}
