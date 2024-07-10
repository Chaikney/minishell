/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstswap_top.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:44:40 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/29 11:44:53 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Exchange the top two elements of stack / linked list.
// First check that there are 2 items to be exchanged.
// then, node1->next = node2->next
// node2->next = &node1
// NOTE This is equivalent to "swap" in push_swap
void	ft_lstswap_top(t_list **stack)
{
	t_list	*node1;
	t_list	*node2;

	if (!(*stack))
		return ;
	node1 = *stack;
	if (node1->next == NULL)
		return ;
	node2 = node1->next;
	node1->next = node2->next;
	node2->next = node1;
	*stack = node2;
}
