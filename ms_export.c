/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 19:22:10 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/01 03:30:10 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Builtin EXPORT command.
// With options, it sets (adds or updates) variables in the
// process environment.
// Without options, it displays those options in a particular format.
void	ms_export_t(t_command *cmd, t_env **envt)
{
	char	*evar_name;
	char	*evar_newvalue;
	int		i;

	i = 1;
	if (cmd->argc < 2)
		ms_export_display_t(*envt);
	else
	{
		while (i <= (cmd->argc - 1))
		{
			evar_name = get_export_name(cmd->argv[i]);
			if ((!evar_name) || (is_legal_name(evar_name) == 0))
				perror ("failed");
			evar_newvalue = get_export_value(cmd->argv[i]);
			if (is_in_envt(evar_name, *envt) == 0)
				t_add_new_env_var(evar_name, evar_newvalue, *envt);
			else
				t_replace_env_var(evar_name, evar_newvalue, envt);
			i++;
			free (evar_name);
			free (evar_newvalue);
		}
	}
}
