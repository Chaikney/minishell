/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 17:06:20 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/16 12:39:47 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	sub_len;
	char	*result;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len || len == 0)
		return (ft_strdup(""));
	sub_len = len;
	if (start + len > s_len)
		sub_len = s_len - start;
	result = malloc(sizeof(char) * (sub_len + 1));
	if (result == NULL)
		return (NULL);
	ft_memcpy(result, s + start, sub_len);
	result[sub_len] = '\0';
	return (result);
}
