/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 19:22:10 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 19:22:11 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FIXME Too many functions in this file. Move some elsewhere!

// Take a name and value and add this  to the envp.
// the Named value should not already exist.
// New version using t_env
void	t_add_new_env_var(char *name, char *value, t_env *env)
{
	char	*to_write;
	t_env	*new_entry;

	to_write = make_env_string(name, value);
	new_entry = init_new_env(to_write);
	add_to_env_list(env, new_entry);
}

// Take a name and value and add this  to the envp.
// the Named value should not already exist.
// If value is NULL, add only the name.
// - Similar to editing pwd,
// - copy envp to somewhere that has space for one more line
// - format and write our full env string.
// - copy the tmp_env back
void	add_new_env_var(char *name, char *value, char **envp)
{
	char	*to_write;
	char	**tmp_envp;
	int		env_len;

	env_len = 0;
	while (envp[env_len] != NULL)
		env_len++;
	tmp_envp = malloc(sizeof(char *) * (env_len + 2));
	if (!tmp_envp)
		return ;
	copy_envp(envp, tmp_envp);
	to_write = make_env_string(name, value);
	tmp_envp[env_len++] = to_write;
	tmp_envp[env_len++] = NULL;
	copy_envp(tmp_envp, envp);
	free(tmp_envp);
}

// Take a name change the value in the envp to the one given.
// TODO If value is NULL, what should replace_env_var do?
// FIXED? This only *adds* values, it does not remove them.
void	replace_env_var(char *name, char *value, char **envp)
{
	int		var_i;

	var_i = find_env_var(envp, name);
	if (var_i == -1)
		printf("\nVariable %s not found to replace", name);	// HACK for debugging
	else
	{
		printf("\nupdating %s with %s", name, value);	// HACK for debugging
		int_unset(name, envp);
		add_new_env_var(name, value, envp);
	}
}

// Return the NAME part of str. OR NULL if the name is invalid.
// NOTE Each argument to EXPORT should be
// in the form NAME=VALUE, without spaces
// therefore, the part to = is the NAME
// See how long the part before = is
// Allocate that space (+ 1 for null terminator)
// Copy backwards to zero
char	*get_export_name(char *str)
{
	int		len;
	char	*name;

	len = 0;
	while ((str[len] != '\0') && (str[len] != '='))
		len++;
	if (len == 0)
		return (NULL);
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (NULL);
	name[len] = '\0';
	while (len > 0)
	{
		len--;
		name[len] = str[len];
	}
	return (name);
}

// return the VALUE part of a NAME=VALUE pair to process in EXPORT
// Variables:
// - value:		the string returned. Must be freed later.
// - midpoint:	index of the = character.
// - len:		Number of chars we have to copy from value.
char	*get_export_value(char *str)
{
	int		midpoint;
	int		len;
	char	*value;

	if (ft_strchr(str, '=') == NULL)
		return (NULL);
	midpoint = 0;
	len = 0;
	while ((str[midpoint] != '\0') && (str[midpoint] != '='))
		midpoint++;
	midpoint++;
	while (str[midpoint + len] != '\0')
		len++;
	value = ft_substr(str, midpoint, len);
	return (value);
}

// A version of ms_export that is easier to understand for me.
// Building from the base.
// - if no parameters, run the display function.
// - for the rest of the parameters
// - - split into NAME and VALUE
// - - Find the NAME in envp using find_env_var
// - - NOTE getenv did *not* work here!
// - - - not there: add new env var
// - - - already there replace env var
// Test cases and expected (bash) behaviour:
// [x] export MS_TEST=hola				Add variable (visible with env)
// [x] export MS_TEST=goodbye			Change previous variable to new value.
// [x] export MS_TEST="hola que tal"	Add 1 variable with spaces	FAIL adds 3 vars
// [x] export MS_TEST hola				adds 2 vars without =
// [x] export hola que tal				as above
// [ ] export MS_TEST=					Add blank "" variable		FAIL env then shows "", unlike bash
// [x] export MS_TEST					Adds var visible export but not env
// [x] export							displays all vars in specific format.
// FIXED There is a problem in replacing OUR, CUSTOM variables.
// ....getenv is not reliable for that!
// FIXME Too many lines (might be the printfs...)
// TODO Adapt to work with t_env instead.
void	ms_export(t_command *cmd, char **envp)
{
	char	*evar_name;
	char	*evar_newvalue;
	int		i;
	int		var_i;

	i = 1;
	if (cmd->argc < 2)
		ms_export_display(envp);
	else
	{
		while (i <= (cmd->argc - 1))
		{
			evar_name = get_export_name(cmd->argv[i]);
			if ((!evar_name) || (is_legal_name(evar_name) == 0))
				perror ("failed");
			printf("\tWill act on: %s", evar_name);	// HACK for debugging
			evar_newvalue = get_export_value(cmd->argv[i]);
			printf("\tto change to: %s", evar_newvalue);	// HACK for debugging
			var_i = find_env_var(envp, evar_name);
			if (var_i == -1)
			{
				printf("variable not found, adding fresh.");	// HACK for debugging
				add_new_env_var(evar_name, evar_newvalue, envp);
			}
			else
			{
				printf("variable exists, replacing (aye right)");	// HACK for debugging
				replace_env_var(evar_name, evar_newvalue, envp);
			}
			i++;
		}
		free (evar_name);
		free (evar_newvalue);
	}
}
