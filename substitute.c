/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 19:04:12 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/01 03:04:03 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Find a variable and add its value into the parameter we are preparing.
// NOTE Special treatment needed for the $? variable as it is discarded
// after printing.
int	add_value_to_par(char **par, int *r_pos, const char *cmd, t_env *envt)
{
	char	*vname;
	char	*vvalue;
	char	*ptr;
	int		name_len;
	int		val_len;

	vname = get_var_name(&cmd[*r_pos]);
	val_len = 0;
	if (!vname)
		return (0);
	name_len = ft_strlen(vname) + 1;
	vvalue = get_value_of_env(vname, envt);
	while (name_len-- > 0)
		(*r_pos)++;
	if (vvalue)
	{
		val_len = ft_strlen(vvalue);
		ptr = vvalue;
		while (*ptr != '\0')
			*(*par)++ = *ptr++;
		if (ft_strncmp(vname, "?", name_len) == 0)
			free (vvalue);
	}
	free (vname);
	return (val_len);
}

// Return the name of a $variable for later substitution
// Returns a null-terminated string *without* the $
// - check we are at $
// - count length (until a space)
// - alloc a string
// - copy characters
// NOTE This does not step over the name in the outer string
char	*get_var_name(const char *str)
{
	int		name_len;
	char	*ptr;
	char	*var_name;

	ptr = (char *) str;
	if (*ptr != '$')
		return (NULL);
	ptr++;
	name_len = 0;
	while ((*ptr != ' ') && (*ptr != '\"') && (*ptr != '\0'))
	{
		name_len++;
		ptr++;
	}
	var_name = malloc(sizeof(char) * (name_len + 1));
	if (!var_name)
		return (NULL);
	var_name[name_len] = '\0';
	while (name_len > 0)
	{
		ptr--;
		var_name[name_len - 1] = *ptr;
		name_len--;
	}
	return (var_name);
}

// Substitute one substring for another in a string
// - calculate new length needed
// - allocate, zero and null-terminate that
// - copy until we reach the $
// - then add the char from new_sub (i.e. the value)
// - step over the variable name
// - copy the rest of the original string
// NOTE We do not free the string passed; that is for the caller
char	*ms_strsub(char *str, char *old_sub, char *new_sub)
{
	int		len;
	char	*new_str;
	char	*cptr;

	if (!old_sub)
		return (str);
	len = ft_strlen(str) - ft_strlen(old_sub) + ft_strlen(new_sub) + 1;
	new_str = malloc(sizeof(char) * len);
	ft_bzero(new_str, len);
	if (!new_str)
		return (NULL);
	new_str[len - 1] = '\0';
	cptr = new_str;
	while (*str != '$')
		*new_str++ = *str++;
	if (new_sub)
		while (*new_sub != '\0')
			*new_str++ = *new_sub++;
	while ((*str != ' ') && (*str != '\0'))
		str++;
	if (*str == ' ')
		str++;
	while (*str != '\0')
		*new_str++ = *str++;
	return (cptr);
}
