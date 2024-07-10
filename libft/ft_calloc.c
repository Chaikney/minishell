/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:31:49 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/12 12:45:07 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Contiguously allocates enough space for
* count objects that are 
* size bytes of memory each 
* returns a pointer to the allocated memory.
* The allocated memory is filled with bytes of value zero.
*
...so this sounds like a specific use of malloc
and also bzero would be used.
FIXED: this fails with zero size allocations:
void *str = ft_calloc(0, 0);
 * Solution was to remove the check of count & size.
 * Seems less safe, but more true to expected behaviour I guess.
*/

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	gimme;
	void	*thisthing;

	gimme = (count * size);
	thisthing = malloc(gimme);
	if ((thisthing != NULL) && (gimme != 0))
		ft_bzero(thisthing, gimme);
	return (thisthing);
}
