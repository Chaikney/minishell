/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:48:20 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/20 10:48:26 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Add the node "new" to the beginning of list "lst".
// Check the list exists, and the node
// (no node = do nothing) (no list, the new node becomes the start of the list)
// Otherwise point the new node's "next" to the current list start
// Then set the list start to be the new node.
// FIXED? if the list is empty, we get a segfault.
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if ((!new) || (!lst))
		return ;
	if (!(*lst))
	{
		*lst = new;
		new->next = NULL;
		return ;
	}
	new->next = *lst;
	*lst = new;
}
