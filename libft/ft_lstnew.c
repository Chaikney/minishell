/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:50:02 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/20 10:50:09 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Create a new node (type t_list) from the supplied content
// ...is this content the *value* of it or the *pointer* to it?
// Returns a pointer to the created t_list (node),
// (the caller may want to use that as their "list start" marker.)
// NOTE to allocate enough memory for this, Sizeof has to be its own type!
t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}
