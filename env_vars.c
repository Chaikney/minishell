/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:42:53 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/01 10:43:06 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Query and manipulate ENV variables

// return 1 if the variable name is present in the env
// This is used in, e.g. int_unset
int	is_in_envt(char *name, t_env *envt)
{
	t_env	*ptr;
	int		len;

	len = ft_strlen(name);
	ptr = envt;
	while (ptr->next != NULL)
	{
		if (ft_strncmp(name, ptr->vname, len) == 0)
			return (1);
		ptr = ptr->next;
	}
	if (ft_strncmp(name, ptr->vname, len) == 0)
		return (1);
	return (0);
}

// Take a name change the value in the envp to the one given.
// TODO If value is NULL, what should replace_env_var do?
void	t_replace_env_var(char *name, char *value, t_env **envt)
{
	t_env	*ptr;
	int		len;

	ptr = *envt;
	len = ft_strlen(name);
	while (ptr)
	{
		if (ft_strncmp(name, ptr->vname, len) == 0)
		{
			if (ptr->value)
				free (ptr->value);
			ptr->value = ft_strdup(value);
			break ;
		}
		ptr = ptr->next;
	}
}

// Take a name and value and add this  to the envp.
// the Named value should not already exist.
// New version using t_env
// HACK This is silly. It creates a string then decomposes it!
void	t_add_new_env_var(char *name, char *value, t_env *env)
{
	char	*to_write;
	t_env	*new_entry;

	to_write = make_env_string(name, value);
	new_entry = init_new_env(to_write);
	add_to_env_list(env, new_entry);
	free (to_write);
}

// return the value of the env. variaable with name.
// NULL if not present.
// Now with the special variable $?
char	*get_value_of_env(char *name, t_env *envt)
{
	t_env	*ptr;
	int		len;

	len = ft_strlen(name);
	if (ft_strncmp(name, "?", len) == 0)
		return (ft_itoa(g_procstatus));
	ptr = envt;
	while (ptr->next != NULL)
	{
		if (ft_strncmp(name, ptr->vname, len) == 0)
			return (ptr->value);
		ptr = ptr->next;
	}
	if (ft_strncmp(name, ptr->vname, len) == 0)
		return (ptr->value);
	return (NULL);
}
