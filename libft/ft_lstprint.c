/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstprint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 14:04:05 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/24 14:04:11 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Take a list and send each item to be printed, with a newline inbetween.
// return the number of list items (nor chars!) printed.
// content is assumed to be a string, don't know if it could be generalised?
int	ft_lstprint(t_list *printme, char delim)
{
	t_list	*activenode;
	int		items_printed;

	items_printed = 0;
	if (!printme)
		ft_printf("Error!\n");
	else
	{
		activenode = printme;
		while (activenode != ft_lstlast(printme))
		{
			if (ft_printf("%s%c", activenode->content, delim) > 0)
				items_printed++;
			activenode = (activenode->next);
		}
		if (ft_printf("%s%c", activenode->content, delim) > 0)
			items_printed++;
	}
	return (items_printed);
}
