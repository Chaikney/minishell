/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 18:56:49 by emedina-          #+#    #+#             */
/*   Updated: 2023/04/19 13:44:32 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	j = 0;
	i = 0;
	while (dst[j])
		j++;
	dst_len = j;
	src_len = ft_strlen(src);
	if (dstsize == 0 || dstsize <= dst_len)
		return (dstsize + src_len);
	while (src[i] && i < dstsize - dst_len - 1)
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (dst_len + src_len);
}
/*
int	main(void)
{
	char	dest[100];
	char	src[];

	dest[100] = "Hello";
	src[] = " World!";
	//size_t str1_len = ft_strlcat(str1, str2, sizeof(str1));
	ft_strlcat(dest, src, 0);
	printf("%s\n", dest);
	return (0);
}
*/
