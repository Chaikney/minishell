/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstcount_num.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:56:43 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/21 10:56:48 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Count the occcurences of integer n in content of a list.
int	ft_lstcount_num(t_list *lst, int n)
{
	int		i;
	t_list	*end;

	i = 0;
	end = ft_lstlast(lst);
	while (lst != end)
	{
		if ((long)(lst->content) == n)
			i++;
		lst = lst->next;
	}
	if ((long) lst->content == n)
		i++;
	return (i);
}
