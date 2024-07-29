/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:47:34 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/18 19:47:39 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Some shell builtin functions, may split later.

// Minishell builtin, our version of pwd.
// Calls getcwd, displays wd then frees the variable.
// NOTE Calling getcwd with NULL and 0 mallocs as much space as is needed
int ms_pwd(void)
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
void	ms_unset_export(char *unset_var, char **envp)
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
	}
	else
		printf("unset: variable %s not found\n", unset_var);
}

// Take a name and value and add this  to the envp.
// the Named value should not already exist.
// If value is NULL, add only the name.
// TODO Implement add_new_env_var
void	add_new_env_var(char *name, char *value, char **envp)
{

}

// Take a name change the value in the envp to the one given.
// TODO If value is NULL, what should replace_env_var do?
// TODO Implement replace_env_var
void	replace_env_var(char *name, char *value, char **envp)
{

}
// Return the NAME part of str. OR NULL if the name is invalid.
// NOTE Each argument to EXPORT should be in the form NAME=VALUE, without(?) spaces
// therefore, the part to = is the NAME
// See how long the part before = is
char	*get_export_name(char *str)
{
	char	*c;
	int	len;
	char	*name;

	len = 0;
	while ((str[len] != '\0') && (str[len] != '='))
		len++;
	if (len == 0)
		return (NULL);
	name = malloc(sizeof(char) * (len + 1 + 1));
	if (!name)
		return (NULL);
	name[len + 1] = '\0';
	while (len >= 0)
	{
		name[len] = str[len];
		len--;
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
void	ms_alt_export(t_command *cmd, char **envp)
{
	char	*evar_name;
	char	*evar_newvalue;
	char	*edit_point;
	int	i;

	i = 1;
	if (cmd->argc < 2)
		ms_export_display(envp);
	else
	{
		while (i < (cmd->argc - 1))
		{
			evar_name = get_export_name(cmd->argv[i]);
			if ((!evar_name) || (is_legal_name(evar_name) == 0))
				perror ("failed");
			evar_newvalue = get_export_value(cmd->argv[i]);
			edit_point = getenv(evar_name);
			if (!edit_point)
			{
				add_new_env_var(evar_name, evar_newvalue, envp);
				// No variable: add a new one.
			}
			else
			{
				replace_env_var(evar_name, evar_newvalue, envp);
				// variable exists: overwrite it
			}
		}


	}

}

// Set a variable in env.
// If the variable does not already exist, add it.
// If the variable name already exists, change it
// 	TODO add notes here to say how we do that: remove existing and then add?
// Method (in my head):
// - read the variable to be changed or set.
// - does it already exist?
// NOTE Each argument to EXPORT should be in the form NAME=VALUE, without(?) spaces
// therefore, the part to = is the NAME
// FIXME ms_export Will have to be shortened to pass Norm
// FIXME ms_export Too many variables for Norm: merge some counters?
// FIXME ms_export does not conform to bash behaviour, see below
// TODO If var name is not followed by = do not change env
// TODO Variable values must not split if commas are used
// TODO export (no args) should print the env like declare -x NAME="VALUE"
// TODO Add checks for permitted variable names using is_legal_name function
// Test cases and expected (bash) behaviour:
// [x] export MS_TEST=hola				Add variable (visible with env)
// [ ] export MS_TEST="hola que tal"	Add 1 variable with spaces	FAIL adds 3 vars
// [ ] export MS_TEST hola				adds 2 vars without =
// [ ] export hola que tal				as above
// [ ] export MS_TEST=					Add blank "" variable		FAIL env then shows "", unlike bash
// [ ] export MS_TEST					No change to env			FAIL adds "blank" variable without =
// [ ] export							displays all vars in specific format.
// Variables used:
// -  unset_var:
void ms_export(t_command *cmd, char **envp) 
{
    char	*var;
    char	*new_var;
    int	len_unset;
    char	**unset_var;	// NOTE Uses malloc, ensure freed later.
    int	len1;
    char	*var2;
    int	len;
    int	j;
    int k;
    int	m;
    int	h;
    size_t	env_len;
    size_t	i;
    char	**new_envp;	// NOTE Uses malloc, ensure freed later.

    len = 0;
    m = 0;
    k= 0;
    len1 = 0;
    j = 0;
    h = 1;
    len_unset = 0;
    if (cmd->argc < 2) 
    {
        ms_export_display(envp);
        return ;
    }
    // Allocates space for ______?
    unset_var = malloc((cmd->argc - 1) * sizeof(char *));
    if (unset_var == NULL) 
    {
        perror("malloc");
        return;
    }
    // NOTE Looks like this depends on parsing? Do we split on = ??
    while(cmd->argv[h] != NULL)
    {
        if(cmd->argv[h][0] == '=')
        {
            var2 = cmd->argv[h];
            len1= ft_strlen(var2);
            var2[len1]= '\0';
            printf("export : %s not a valid identifier",var2);
            h++;
        }
        var = cmd->argv[h];
        while (cmd->argv[h][len_unset] != '=' && cmd->argv[h][len_unset] != '\0')
            len_unset++;
        unset_var[k] = malloc(sizeof(char ) * len_unset + 1);
        if (unset_var[k] == NULL) 
        {
            perror("malloc");
            while (m < k)
                free(unset_var[m++]);
            free(unset_var);
            return;
        }
        while (j <= len_unset - 1)
        {
            unset_var[k][j] = var[j];
            j++;
        }
        unset_var[k][len_unset] = '\0';
        ms_unset_export(unset_var[k],envp);
        env_len = 0;
        j = 0;
        k++;
        while (envp[env_len] != NULL)
            env_len++;
        new_envp = malloc(sizeof(char *) * (env_len + 2));
        if (new_envp == NULL)
            {
                perror("malloc");
                free(new_envp);
                m = 0;
                while (m <= k)
                    free(unset_var[m++]);
                free(unset_var);
                return;
            }

        i = 0;
        while (i < env_len)
        {
            new_envp[i] = envp[i];
            i++;
        }

        new_var = ft_strdup(var);
        if (new_var == NULL)
            {
                perror("strdup");
                free(new_envp);
                int m = 0;
                while (m <= k)
                    free(unset_var[m++]);
                free(unset_var);
                return;
            }
        len = ft_strlen(new_var);
        if(new_var[len - 1] == '=')
            new_var = ft_strjoin(new_var,"\"\"");
       if (new_var == NULL)
            {
                perror("ft_strjoin");
                free(new_envp);
                m = 0;
                while (m <= k)
                    free(unset_var[m++]);
                free(unset_var);
                return;
            }
        new_envp[env_len] = new_var;
        new_envp[env_len + 1] = NULL;
        i = 0;
        while (i < env_len + 1)
        {
            envp[i] = new_envp[i];
            i++;
        }
        envp[env_len + 1] = NULL;

        free(new_envp);
        h++;
        len_unset = 0;
    }
}


// This is UNSET as called directly by the user.
// Remove the variable in cmd from ENV, if present.
// If not present, take no action.
// TEST cases and expected (bash) behaviour:
// [x] unset a variable that we set previously using export
// [x] unset a variable that was inherited
// FIXED ms_unset should not segfault
// TODO Use perror not printf when cmd fails?
// TODO If this fails, should we set g_procstatus?
// TODO Do we need to free memory for var_name?
// FIXME ms_unset has too many lines
void	ms_unset(t_command *cmd, char **envp)
{
	int	i;
	int	var_index;
	char	*var_name;

	i = 0;
	if (cmd->argc < 2)
	{
		printf("unset: missing argument\n");
		return ;
	}
	i = ft_strlen(cmd->argv[1]);
	var_name = malloc(sizeof (char *) * i);	// TODO Do we need to have + 1 for null-termination here?
	var_name = cmd->argv[1];
	var_index = find_env_var(envp, var_name);
	if (var_index >= 0)
	{
		// Liberar la memoria de la variable de entorno
//        free(envp[var_index]);	// NOTE Segfault caused here - this is invalid when we did not allocate the var
		// Eliminar la variable de entorno del arreglo envp
		i = var_index;
		while (envp[i] != NULL)
		{
			envp[i] = envp[i + 1];
			i++;
		}
	}
	else
		printf("unset: variable %s not found\n", var_name);
}
