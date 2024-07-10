/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stackpop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:46:35 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/29 11:46:39 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Remove the top item from a stack / linked list
// NOTE the node is freed.
void	ft_stackpop(t_list *stack)
{
	t_list	*tmp;

	if (stack == NULL)
		return ;
	else
	{
		tmp = stack;
		stack = stack->next;
		free (tmp);
	}
}
