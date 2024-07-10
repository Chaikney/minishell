/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:49:50 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/20 10:49:54 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// lst: address to a pointer to a node
// f: address of the function to apply to the lst.
// del: address of a function to delete node content.
// Applies f to the content of each node of lst.
// Creates (and returns) a new list resulting from application of f
// Returns NULL if allocation fails.
// del is used to delete the content of a node if needed.
// So if something fails, use it on the node?
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void*))
{
	t_list	*newnode;
	t_list	*startnode;

	if (!lst && !(*f) && !(*del))
		return (NULL);
	startnode = NULL;
	while (lst)
	{
		newnode = ft_lstnew ((*f)(lst->content));
		if (!newnode)
		{
			(*del)(lst->content);
			break ;
		}
		ft_lstadd_back(&startnode, newnode);
		lst = lst->next;
	}
	return (startnode);
}
