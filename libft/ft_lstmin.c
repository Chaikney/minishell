/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:47:35 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/23 10:47:53 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Read a list of numbers
// Return the minimum.
// If the list is empty, return 0.
long	ft_lstmin(t_list *stack)
{
	t_list	*stackptr;
	t_list	*last;
	long	min;

	if (!stack)
		return (0);
	stackptr = stack;
	last = ft_lstlast(stack);
	min = (long)stackptr->content;
	while (stackptr != last)
	{
		stackptr = stackptr->next;
		if ((long)stackptr->content < min)
			min = (long)stackptr->content;
	}
	if ((long)stackptr->content < min)
		min = (long)stackptr->content;
	return (min);
}
