#include "minishell.h"


void swap(t_env *a, t_env *b)
{
	char *temp_vname = a->vname;
	char *temp_value = a->value;
	a->vname = b->vname;
	a->value = b->value;
	b->vname = temp_vname;
	b->value = temp_value;
}
void sort_env(t_env *head)
{
	if (head == NULL)
		return;
	int swapped = 1;
	t_env *ptr1;
	t_env *lptr = NULL;
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
void append(t_env **head_ref, char *new_vname, char *new_value)
{
	t_env *new_node = (t_env *)malloc(sizeof(t_env));
	t_env *last = *head_ref;
	new_node->vname = new_vname;
	new_node->value = new_value;
	new_node->next = NULL;
	if (*head_ref == NULL)
	{
		*head_ref = new_node;
		return;
	}
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
}
void print_list(t_env *node)
{
	while (node != NULL)
	{
		printf("vname: %s, value: %s\n", node->vname, node->value);
		node = node->next;
	}
	}
