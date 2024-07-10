/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstrotate_rev.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:45:19 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/29 11:45:23 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Shift down all elements of the stack by one.
// The last becomes the first.
// NOTE this is like a circle that you snip to define start and end.
// Check that there are more than one nodes in stack
// find the end of the list, and the penultimate
// set the second last node to be the new end
// "Move" the last node by pointing to the start as next
// ...and setting the new stack-start to the old last node.
void	ft_lstrotate_rev(t_list **stack)
{
	t_list	*original_last;
	t_list	*penultimatenode;

	if (ft_lstsize(*stack) < 2)
		return ;
	original_last = ft_lstlast(*stack);
	penultimatenode = *stack;
	while (penultimatenode->next != original_last)
		penultimatenode = penultimatenode->next;
	(penultimatenode)->next = NULL;
	original_last->next = *stack;
	*stack = original_last;
}
