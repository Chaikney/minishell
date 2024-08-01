/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:39:10 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 12:39:13 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Turn the t_env variables back into a list of string like envp
// This means that they would work with execve.
// NOTE Untested, but shoulld be used to  pass to run_command etc
char	**serialise_env(t_env *env)
{
	char	**env_list;
	t_env	*ptr;
	int		len;
	int	i;

	len = 0;
	ptr = env;
	while (ptr->next != NULL)
	{
		len++;
		ptr = ptr->next;
	}
	len++;
	env_list = malloc (sizeof(char *) * (len + 1));
	i = 0;
	while (env->next != NULL)
	{
		env_list[i++] = make_env_string(env->vname, env->value);
		env = env->next;
	}
	env_list[i] = NULL;
	return (env_list);
}

// The ENV builtin, but using a t_env
// Does not display "empty" variables (unlike export)
void	ms_env_t(t_env *environ)
{
	t_env	*ptr;

	ptr = environ;
	while (ptr->next != NULL)
	{
		if (ptr->value)
			printf("%s=%s\n", ptr->vname, ptr->value);
		ptr = ptr->next;
	}
	if (ptr->value)
		printf("%s=%s\n", ptr->vname, ptr->value);
}

// Sets up a new t_env node to be added
// NOTE This function is best suited for the initial parsing.
// Perhaps a version where the vname and value are already split
// would work better sometimes.
t_env	*init_new_env(char *str)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));	// FIXED? Memory allocated here is not freed
	new_env->next = NULL;
	new_env->vname = get_export_name(str);
	new_env->value = get_export_value(str);
	return (new_env);
}

// Adds a t_env node to the set.
void	add_to_env_list(t_env *lsthead, t_env *to_add)
{
	t_env	*ptr;

	ptr = lsthead;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = to_add;
}

// Takes a  name and a value; returns a string formatted suitable
// for storing as an environment variable.
// Variables:
// len:	length needed for string. 1 for null and 1 for the =
// name:	of the environment variable
// value:	of the environment variable
// Used for display and serialising purposes now.
char	*make_env_string(char *name, char *value)
{
	char	*env_str;
	char	*tmp;

	if (!name)
		return (NULL);
	if (!value)
		return (ft_strdup(name));
	else
	{
		tmp = ft_strjoin(name, "=");
	}
	env_str = ft_strjoin(tmp, value);
	free (tmp);
	return (env_str);
}
