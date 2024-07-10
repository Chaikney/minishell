/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstrotate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:45:09 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/29 11:45:13 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Shift up all elements of the stack by one.
// The first becomes the last.
	// Check that there are more than one nodes in stack
	// find the end of the list
	// break the first node away from the rest of the sequence
	// Tell the last node that there's something after it (the old 1st node)
	// This should be setting the new stack-start to the old 2nd node.
void	ft_lstrotate(t_list **stack)
{
	t_list	*original_last;
	t_list	*secondnode;

	if ((*stack)->next == NULL)
		return ;
	original_last = ft_lstlast(*stack);
	secondnode = (*stack)->next;
	(*stack)->next = NULL;
	original_last->next = *stack;
	*stack = secondnode;
}
