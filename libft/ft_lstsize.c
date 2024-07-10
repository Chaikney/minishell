/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:50:18 by chaikney          #+#    #+#             */
/*   Updated: 2023/10/20 10:50:23 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//Takes a pointer to a list and returns the size of that list.
//Or, takes a pointer to the first *node* in the list, rather.
int	ft_lstsize(t_list *lst)
{
	int		count;
	t_list	*location;

	count = 0;
	if (lst == NULL)
		return (count);
	else
		location = lst;
	while (location->next != NULL)
	{
		count += 1;
		location = location->next;
	}
	count += 1;
	return (count);
}
