/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 12:22:15 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/18 10:59:44 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	absolute_value(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

static int	get_len(int n)
{
	int	len;

	len = 0;
	if (n < 0)
		len++;
	else if (n == 0)
		len++;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		i;
	int		len;
	char	*result;

	i = 0;
	len = get_len(n);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	if (n < 0)
	{
		result[i++] = '-';
		n = -n;
	}
	else if (n == 0)
		result[i++] = '0';
	while (n != 0)
	{
		result[--len] = absolute_value(n % 10) + '0';
		n = n / 10;
		i++;
	}
	result[i] = '\0';
	return (result);
}

/* int main()
{
	int n=-155;
	
	printf("%s",ft_itoa(n));
} */