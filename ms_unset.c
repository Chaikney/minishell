/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:19:21 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/08 13:19:23 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// UNSET used internally by other minishell functions (cd, export)
// This takes the name of a variable to be unset,
// whereas user-facing ms_export takes a t_command.
// NOTE Potential to unify these!
// ...what do we mean by "unset"? AND how does this work?
// - Finds the line in envp where the variable is.
// - copies the next lines over it.
// FIXED I think that variables being unset should be freed.
// TODO Do we need to set g_procstatus on node not found?
void	int_unset(char *unset_var, t_env *enviro)
{
	t_env	*to_unset;
	int		len;

	if (is_in_envt(unset_var, enviro) == 1)
	{
		to_unset = enviro;
		len = ft_strlen(unset_var);
		while ((to_unset)
			&& ft_strncmp(to_unset->vname, unset_var, len) != 0)
			to_unset = to_unset->next;
		if (to_unset)
		{
			remove_node(&enviro, to_unset);
			free (to_unset->vname);
			if (to_unset->value)
				free (to_unset->value);
			free(to_unset);
		}
		else
			printf("could not find node to unset");
	}
}

// Handle errors for unset
static void	unset_error(char *errmsg, int err)
{
	printf("%s\n", errmsg);
	g_procstatus = err;
}

// This is UNSET as called directly by the user.
// Remove the variable in cmd from ENV, if present.
// If not present, take no action.
// TEST cases and expected (bash) behaviour:
// [x] unset a variable that we set previously using export
// [x] unset a variable that was inherited
void	ms_unset(t_command *cmd, t_env *enviro)
{
	int		i;
	char	*var_name;

	i = 1;
	g_procstatus = 0;
	if (cmd->argc < 2)
		unset_error("unset: missing argument", EINVAL);
	else
	{
		while (i <= (cmd->argc - 1))
		{
			var_name = get_export_name(cmd->argv[i]);
			if ((!var_name) || (is_legal_name(var_name) == 0))
				unset_error("unset failed", ENOMEM);
			else if (is_legal_name(var_name) == 0)
				unset_error("Bad variable name", EINVAL);
			else if (is_in_envt(var_name, enviro) == 1)
				int_unset(var_name, enviro);
			i++;
			free (var_name);
		}
	}
}
