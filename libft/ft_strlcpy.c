/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 18:55:23 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/16 11:17:34 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	aux;

	i = 0;
	aux = 0;
	while (src[aux])
	{
		aux++;
	}
	if (dstsize < 1)
	{
		return (aux);
	}
	while (src[i] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (aux);
}

/* int	main(void)
{
	char	destino[10];
	char	*origen;
	size_t	num_copied;

	origen = "Hola, mundo!";

	num_copied = ft_strlcpy(destino, origen, sizeof(destino));
	
	printf("La cadena de origen es: %s\n", origen);
	printf("La cadena de destino es: %s\n", destino);
	printf("Se copiaron %zu caracteres\n", num_copied);
	return (0);
} */
