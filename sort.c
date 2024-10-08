/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 19:07:37 by emedina-          #+#    #+#             */
/*   Updated: 2024/08/01 03:17:46 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap(t_env *a, t_env *b)
{
	char	*temp_vname;
	char	*temp_value;

	temp_vname = a->vname;
	temp_value = a->value;
	a->vname = b->vname;
	a->value = b->value;
	b->vname = temp_vname;
	b->value = temp_value;
}

void	sort_env(t_env *head)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr;

	if (head == NULL)
		return ;
	swapped = 1;
	lptr = NULL;
	while (swapped)
	{
		swapped = 0;
		ptr1 = head;
		while (ptr1->next != lptr)
		{
			if (strcmp(ptr1->vname, ptr1->next->vname) > 0)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
}

void	append(t_env **head_ref, char *new_vname, char *new_value)
{
	t_env	*new_node;
	t_env	*last;

	new_node = (t_env *)malloc(sizeof(t_env));
	last = *head_ref;
	new_node->vname = new_vname;
	new_node->value = new_value;
	new_node->next = NULL;
	if (*head_ref == NULL)
	{
		*head_ref = new_node;
		return ;
	}
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
}

// Removes a t_env from our environment list.
// It does not free the node, the caller must do that.
void	remove_node(t_env **first, t_env *target)
{
	t_env	*before;
	t_env	*ptr;

	before = *first;
	ptr = before->next;
	while ((ptr->vname != target->vname) && (ptr->next != NULL))
	{
		before = before->next;
		ptr = ptr->next;
	}
	if (before->next == NULL)
		exit(EXIT_FAILURE);
	if (target != *first)
		before->next = target->next;
	else
		*first = target->next;
}

void	print_list(t_env *node)
{
	while (node != NULL)
	{
		printf("vname: %s, value: %s\n", node->vname, node->value);
		node = node->next;
	}
}
