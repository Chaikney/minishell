/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:47:56 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/20 10:48:03 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Deletes (with "del") and frees:
// * the given node; and
// * every successor of that node.
// Finally sets as NULL the pointer to the list.
// "lst" is the address of a pointer to a node.
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*thenyou;

	if (!lst)
		return ;
	if (del && lst)
	{
		while (*lst)
		{
			thenyou = (*lst)->next;
			ft_lstdelone(*lst, del);
			*lst = thenyou;
		}
	}
	lst = NULL;
}
