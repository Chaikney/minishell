/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:46:41 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/23 10:47:28 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Read a list of numbers
// Return the maximum.
// If the list is empty, return 0.
// DONE Add tests for lstmax
// DONE Use lstlast instead of != NULL, avoid looping.
// FIXED There is a segfault here caused by the test file.
long	ft_lstmax(t_list *stack)
{
	t_list	*stackptr;
	t_list	*last;
	long	max;

	if (!stack)
		return (0);
	last = ft_lstlast(stack);
	stackptr = stack;
	max = (long)stackptr->content;
	while (stackptr != last)
	{
		stackptr = stackptr->next;
		if ((long)stackptr->content > max)
			max = (long)stackptr->content;
	}
	if ((long)stackptr->content > max)
		max = (long)stackptr->content;
	return (max);
}
