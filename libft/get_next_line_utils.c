/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 14:02:05 by chaikney          #+#    #+#             */
/*   Updated: 2023/08/21 14:02:27 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Returns a new string from 2 NT'd strings
// Returns NULL on allocation error
// Frees the first string given.
//NOTE compared with ft_strjoin:
// This creates a new str and frees the old one.
// The syntax is more concise
// if s1 does not exist, it creates it
// if alloc fails, it destroys s1
// does not use const (what does that mean)
// ==> the semantics are slightly different, keep 2 versions for now.
char	*gnl_strjoin(char *s1, char *s2)
{
	char	*newstr;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!s1)
	{
		s1 = malloc(sizeof(char));
		if (!s1)
			return (NULL);
		else
			s1[0] = '\0';
	}
	newstr = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (newstr == NULL)
		return (free (s1), NULL);
	while (s1[++i] != '\0')
		newstr[i] = s1[i];
	while (s2[j] != '\0')
		newstr[i++] = s2[j++];
	newstr[i] = '\0';
	free(s1);
	return (newstr);
}
