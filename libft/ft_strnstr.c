/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:33:06 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/10 11:32:21 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Locates the first occurrence of the null-terminated string *needle* in the
string *haystack*, where not more than *len* characters are searched.
Characters that appear after a `\0' character are not searched.

If needle is an empty string, haystack is returned;
if needle occurs nowhere in haystack, NULL is returned; otherwise
a pointer to the first character of the first occurrence of needle is returned

 * DONE: only test fail atm is when len is -1. What *should* happen??
 * 	it causes an *overflow* and so its "real" value becomes close to INT_MAX
 * 	should I cast it? Treat it as an absolute? Reject it?
 * 	Almost certainly, it means needle not found.
 * 	Simply CASTing to int in len check solves some problems
 * 	but creates/reveals others.
 * 	Solution (in terms of passing tests) was to remove the len < 0 and let
 * 		it sort itself out :shrug:
	DONE: an empty haystack should return NULL
*/

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char				*here;
	unsigned int		needlength;
	unsigned int		i;

	i = 0;
	if (*needle == '\0')
		return ((char *)(haystack));
	here = ft_strchr(haystack, needle[0]);
	needlength = ft_strlen(needle);
	if ((here == NULL) || (needlength > len) \
			|| (*haystack == '\0'))
		return (NULL);
	while ((*haystack != '\0') && ((len - needlength) >= i))
	{
		if ((haystack[0] == needle[0]) && \
				(ft_strncmp(haystack, needle, needlength) == 0))
			return ((char *)(haystack));
		haystack++;
		i++;
	}
	return (NULL);
}
