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

// The more complicated / slower parsing that we need to do to identify
// a builtin.
static t_builtin	detailed_parse_builtin(t_command *cmd, int posn)
{
	int	len;

	len = ft_strlen(cmd->argv[posn]);
	if ((ft_strncmp(cmd->argv[posn], "exit", 4) == 0) && (len == 4))
		return (EXIT);
	else if ((ft_strncmp(cmd->argv[posn], "unset", 5) == 0) && (len == 5))
		return (UNSET);
	else if ((ft_strncmp(cmd->argv[posn], "export", 6) == 0) && (len == 6))
		return (EXP);
	else if ((ft_strncmp(cmd->argv[posn], "echo", 4) == 0) && (len == 4))
	{
		if ((cmd->argv[posn + 1]) && (ft_strlen(cmd->argv[posn + 1]) == 2)
			&& (ft_strncmp(cmd->argv[posn + 1], "-n", 2) == 0))
			return (ECHON);
		else
			return (ECHO);
	}
	return (NONE);
}

// Read the given command argument and if it matches a builtin, set the flag.
// (posn will be 0 except when there is input redirection)
// First a sanity check on argument annd position,
// then we check whether the first letter matches one of the builtin names.
// then the shorter names.
// If more complicated parsing is needed, we call detailed_parse_builtin
// NOTE We no longer check if the command has extra arguments
// - the builtin can just ignore them
// FIXED Segfaults here given ><
// It splits to 2 tokens but one command - should not happen?
// but how to guard against?
// FIXME Still a problem if >© is given.
t_builtin	parse_builtin(t_command *cmd, int posn)
{
	t_builtin	retvalue;
	int			len;

	if ((cmd->argc == 0) || (cmd->argc < posn))
		retvalue = (NONE);
	else
	{
		len = ft_strlen(cmd->argv[posn]);
		if (ft_strchr("cepu", cmd->argv[posn][0]) == NULL)
			retvalue = (NONE);
		else if ((ft_strncmp(cmd->argv[posn], "cd", 2) == 0) && (len == 2))
			retvalue = (CD);
		else if ((ft_strncmp(cmd->argv[posn], "pwd", 3) == 0) && (len == 3))
			retvalue = PWD;
		else if ((ft_strncmp(cmd->argv[posn], "env", 3) == 0) && (len == 3))
			retvalue = ENV;
		else
			retvalue = detailed_parse_builtin(cmd, posn);
	}
	return (retvalue);
}

// Minishell builtin, our version of pwd.
// Calls getcwd, displays wd then frees the variable.
// NOTE Calling getcwd with NULL and 0 mallocs as much space as is needed
int	ms_pwd(void)
{
	char	*wd;

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
// FIXED I think that variables being unset should be freed.
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
	if (cmd->argc < 2)
	{
		perror("unset: missing argument\n");
		g_procstatus = EINVAL;
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
			if (is_in_envt(var_name, enviro) == 1)
				int_unset(var_name, enviro);
			i++;
			free (var_name);
		}
	}
}
