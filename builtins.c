/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:47:34 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/31 20:06:59 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Some shell builtin functions, may split later.

// Minishell builtin, our version of pwd.
// Calls getcwd, displays wd then frees the variable.
// NOTE Calling getcwd with NULL and 0 mallocs as much space as is needed
int	ms_pwd(void)
{
	char    *wd;

	wd = NULL;
	wd = getcwd(wd, 0);
	if (!wd)
		return (-1);
	printf("%s\n", wd);
	free (wd);
	return (0);
}

// UNSET used internally by other minishell functions (cd, export)
// This takes the name of a variable to be unset,
// whereas user-facing ms_export takes a t_command.
// NOTE Potential to unify these!
// ...what do we mean by "unset"? AND how does this work?
// - Finds the line in envp where the variable is.
// - copies the next lines over it.
// FIXME I think that variables being unset should be freed.
void	int_unset(char *unset_var, char **envp, t_env *enviro)
{
	int	var_index;

	var_index = 0;
	var_index = find_env_var(envp, unset_var);
	if (var_index >= 0)
	{
		while (envp[var_index] != NULL)
		{
			envp[var_index] = envp[var_index + 1];
			var_index++;
		}
		delete_node(&enviro, unset_var);
	}
	else
		printf("unset: variable %s not found\n", unset_var);
}

// This is UNSET as called directly by the user.
// Remove the variable in cmd from ENV, if present.
// If not present, take no action.
// TEST cases and expected (bash) behaviour:
// [x] unset a variable that we set previously using export
// [x] unset a variable that was inherited
// FIXED ms_unset should not segfault
// DONE Use perror not printf when cmd fails?
// TODO If this fails, should we set g_procstatus?
// TODO Do we need to free memory for var_name?
// FIXED ms_unset has too many lines
// DONE UNSET should act to act on multiple vars, space separated,.
// NOTE This *works* but I don't understand why...
// - Make variable name from cmd parameter
// - Look for the name in ENVP
// - - if not there, do nothing
// - - if there:
// - - copy the next line over this one, and so on to end of envp
void	ms_unset(t_command *cmd, char **envp, t_env *enviro)
{
	int	i;
	char	*var_name;

	i = 1;
	if (cmd->argc < 2)
	{
		perror("unset: missing argument\n");
		g_procstatus = EINVAL;
		return ;
	}
	else
	{
		while (i <= (cmd->argc - 1))
		{
			var_name = get_export_name(cmd->argv[i]);
			if ((!var_name) || (is_legal_name(var_name) == 0))
			{
				perror ("unset failed");
				g_procstatus = ENOMEM;
			}
			int_unset(var_name, envp, enviro);
			i++;
		}
	}
}
