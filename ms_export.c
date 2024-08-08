/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 19:22:10 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/03 10:46:11 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// EXPORT with no options displays an ordered list of the
// environment variables in a specific format.
void	ms_export_display_t(t_env *envt)
{
	t_env	*ptr;

	sort_env(envt);
	ptr = envt;
	while (ptr->next != NULL)
	{
		printf("declare -x %s", ptr->vname);
		if (ptr->value)
			printf("=\"%s""\"\n", ptr->value);
		else
			printf("\n");
		ptr = ptr->next;
	}
	printf("declare -x %s", ptr->vname);
	if (ptr->value)
		printf("=\"%s""\"\n", ptr->value);
	else
		printf("\n");
}

static void	export_error(char *errmsg, int err)
{
	printf("%s\n", errmsg);
	g_procstatus = err;
}

// sets (adds or updates) variables in the
// process environment .
// Splits the parameter into value and name parts
// If the name does not exist, adds it.
// If the name already exists, unsets then adds it.
static void	export_single_var(char *par, t_env **envt)
{
	char	*evar_name;
	char	*evar_newvalue;

	evar_name = get_export_name(par);
	evar_newvalue = get_export_value(par);
	if ((!evar_name) || (is_legal_name(evar_name) == 0))
		export_error("failed", 1);
	else
	{
		if (is_in_envt(evar_name, *envt) == 0)
			t_add_new_env_var(evar_name, evar_newvalue, *envt);
		else
			t_replace_env_var(evar_name, evar_newvalue, envt);
	}
	free (evar_name);
	free (evar_newvalue);
}

// Guiding function for Builtin EXPORT command.
// With options, it sets (adds or updates) variables in the
// process environment via export_single_var.
// Without options, it displays those options in a particular format.
void	ms_export_t(t_command *cmd, t_env **envt)
{
	int		i;

	i = 1;
	g_procstatus = 0;
	if (cmd->argc < 2)
		ms_export_display_t(*envt);
	else
	{
		while (i <= (cmd->argc - 1))
		{
			export_single_var(cmd->argv[i], envt);
			i++;
		}
	}
}
