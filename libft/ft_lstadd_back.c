/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:47:17 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/20 10:47:29 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Add the node "new" to the end of list "lst"
// find the end of the list
// change that node's "next" to the address of "new"
// Make sure we have been given a list and a node.
// NOTE Understand the pointer dynamics here!
// **lst is a pointer to the pointer that is(? points to?) the list start
// ...that's why we check for the existence of *lst
// *new is a pointer to the new node, which we must manipulate
// FIXED? Must ensure that the list ends with a next==NULL!
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if ((!new) || (!lst))
		return ;
	if (new->next != NULL)
		new->next = NULL;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	if (!last)
		return ;
	last->next = new;
}
