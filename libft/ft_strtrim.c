/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:39:33 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/18 17:13:32 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len1;
	size_t	i;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	len1 = ft_strlen(s1);
	i = 0;
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	while (len1 > 0 && ft_strchr(set, s1[len1 - 1]))
		len1--;
	if (len1 < i)
		len1 = i;
	str = malloc(len1 - i + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1 + i, len1 - i + 1);
	return (str);
}
/*  int main(void)
 {
	char const *str = NULL;
	char const *str2 = NULL;
	
	char *pepe;
	pepe=ft_strtrim(str, str2);
	printf("test %s",pepe);
 } */