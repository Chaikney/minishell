/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:48:48 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/20 10:48:54 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Takes node "lst" as a parameter and uses the
// function "del" to free the memory of the node's content
// and free the node.
// "The memory of 'next' must not be freed" ?
// NOTE I'm not 100% sure what gets removed here.
// Do we only, here, have .next and .content? Surely this
// has to be generalisable to multiple things.
// OR, is content a pointer to some kind of itemset
// and we have a "delete node content" function passed in?
void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	(*del)(lst->content);
	free (lst);
}
