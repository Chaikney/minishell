/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsorted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:48:01 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/23 10:48:13 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Read an integer list, return an integer showing its sort direction.
// Return 1 if sorted in ascending order.
// Return 0 if unsorted.
// Return -1 if sorted in descending order.
int	ft_lstsorted(t_list *stack)
{
	long	prev;
	int		direction;
	t_list	*node;
	t_list	*last;

	node = stack;
	if (node->next == NULL)
		return (0);
	prev = (long) node->content;
	last = ft_lstlast(stack);
	direction = ((long) last->content - prev);
	while ((node->next != NULL))
	{
		node = node->next;
		if (((((long) node->content - prev) <= 0) && (direction > 0)) \
			|| (((((long) node->content) - prev) >= 0) && (direction < 0)))
			return (0);
		prev = (long) node->content;
	}
	if (direction > 0)
		return (1);
	if (direction < 0)
		return (-1);
	return (0);
}
