/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpush_to.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:44:02 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/29 11:44:11 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Take the top element of src and place it at the top of dest.
// NOTE n push_swap the definition of "push a" is like "push TO a"
// If src is empty, we do nothing there is nothing to push.
// src goes to the top of dest using lstadd_front
// (Dest can be empty but must have been initialised)
// ...and this changes dest stacktop reference
// then we alter the src stacktop reference.
// the old src->next becomes top of src
// NOTE Careful with leaving src empty. Is valid but dangerous.
// NOTE second_node could be set to NULL, don't act on it.
void	ft_lstpush_to(t_list **src, t_list **dest)
{
	t_list	*mv_node;
	t_list	*second_node;

	if ((src == NULL) || (dest == NULL))
		return ;
	mv_node = *src;
	second_node = mv_node->next;
	ft_lstadd_front(dest, mv_node);
	*src = second_node;
}
