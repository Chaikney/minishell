/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:25:39 by emedina-          #+#    #+#             */
/*   Updated: 2023/05/18 16:24:51 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	void	*ptr;

	new_node = NULL;
	while (lst && f && del)
	{
		ptr = f(lst->content);
		new_list = ft_lstnew(ptr);
		if (!new_list)
		{
			free (ptr);
			ft_lstclear(&new_list, del);
		}
		ft_lstadd_back(&new_node, new_list);
		lst = lst->next;
	}
	return (new_node);
}
