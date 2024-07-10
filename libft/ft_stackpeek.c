/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stackpeek.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:46:21 by chaikney          #+#    #+#             */
/*   Updated: 2023/11/29 11:46:27 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Return the content (assumed to be int) at the top of a stack.
// If the stack is empty return null char.
int	ft_stackpeek(t_list *stack)
{
	if (stack != NULL)
		return ((long) stack->content);
	else
		return ('\0');
}
