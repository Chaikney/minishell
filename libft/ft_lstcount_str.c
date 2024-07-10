/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstcount_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:56:25 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/21 10:56:37 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Count the occcurences of string str in content of a list.
int	ft_lstcount_str(t_list *lst, char *str)
{
	int		i;
	t_list	*end;

	i = 0;
	end = ft_lstlast(lst);
	while (lst != end)
	{
		if (lst->content == str)
			i++;
		lst = lst->next;
	}
	if (lst->content == str)
		i++;
	return (i);
}
