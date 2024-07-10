/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsnip.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:57:03 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/21 10:57:16 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Remove node from list, repair the gap left by removing the node.
// Find the node before node. Mark it.
// Change its ->next to be the same as node's
// Delete the node
// NOTE Have removed the delone because (I think) I don't have to free it(?)
// ...I have big doubts though. Surely its mallocd somewhere?!
void	ft_lstsnip(t_list **lst, t_list *node)
{
	t_list	*before;
	t_list	*ptr;

	before = *lst;
	ptr = before->next;
	while ((ptr->content != node->content) && (ptr->next != NULL))
	{
		before = before->next;
		ptr = ptr->next;
	}
	if (before->next == NULL)
		exit (EXIT_FAILURE);
	if (node != *lst)
		before->next = node->next;
	else
		*lst = node->next;
}
