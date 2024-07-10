/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 15:52:41 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/04 17:54:03 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
DESCRIPTION
Lexicographically compare the null-terminated strings s1 and s2.
The strncmp() function compares not more than n characters.
Because strncmp() is designed for comparing strings rather than binary data,
characters that appear after a `\0' character are not compared.

Return an integer greater than, equal to, or less than 0,
according as the string s1 is greater than, equal to, or less than the string s2.
Compare using unsigned characters, so that `\200' is greater than `\0'.  
 */

int	ft_strncmp(char *s1, char *s2, unsigned int n)
{
	unsigned char	comp1;
	unsigned char	comp2;

	while ((n > 0) && ((*(unsigned char *) s2) == (*(unsigned char *) s1)) \
			&& (*s1 != '\0'))
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	comp1 = (*(unsigned char *) s1);
	comp2 = (*(unsigned char *) s2);
	return (comp1 - comp2);
}
