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


//Variables:
//len:	length needed for string. 1 for null and 1 for the =
//name:	of the environment variable
//value:	of the environment variable
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

// Take a name and value and add this  to the envp.
// the Named value should not already exist.
// If value is NULL, add only the name.
// TODO Implement add_new_env_var
// - Similar to editing pwd, copy envp to somewhere that has space for one more line
// - format and write our full env string.
void	add_new_env_var(char *name, char *value, char **envp)
{
	char	*to_write;
	char	**tmp_envp;
	int	env_len;

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
// TODO Implement replace_env_var
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
		ms_unset_export(name, envp);
		add_new_env_var(name, value, envp);
	}
}

// Return the NAME part of str. OR NULL if the name is invalid.
// NOTE Each argument to EXPORT should be in the form NAME=VALUE, without(?) spaces
// therefore, the part to = is the NAME
// See how long the part before = is
// FIXED Returns the = along with the name.
char	*get_export_name(char *str)
{
	int	len;
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
// - value:	the string returned. Must be freed later.
// - midpoint:	index of the = character.
// - len:		NUmber of chars we have to copy from value.
// NOTE: Could we just give len as some large amount and let substr handle it for us?
char	*get_export_value(char *str)
{
	int	midpoint;
	int	len;
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
// - - Find the NAME in envp using getenv FIXME this method doesn't work
// - - - not there: add new env var
// - - - already there replace env var
// Test cases and expected (bash) behaviour:
// [x] export MS_TEST=hola				Add variable (visible with env)
// [x] export MS_TEST=goodbye			Change previous variable to new value.
// FIXME test below fails because splits the value as if 2 further vars.
// [ ] export MS_TEST="hola que tal"	Add 1 variable with spaces	FAIL adds 3 vars
// [x] export MS_TEST hola				adds 2 vars without =
// [x] export hola que tal				as above
// [ ] export MS_TEST=					Add blank "" variable		FAIL env then shows "", unlike bash
// [x] export MS_TEST					Adds var visible export but not env
// [x] export							displays all vars in specific format.
// FIXED There is a problem in replacing OUR, CUSTOM variables.
// ....getenv is not reliable for that!
void	ms_alt_export(t_command *cmd, char **envp)
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
        free (evar_name);	// FIXME Segfault here if export MS_TEST
        free (evar_newvalue);	// FIXME Similarly sketchy memory behaviour.
	}
}
