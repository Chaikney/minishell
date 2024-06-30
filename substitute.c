/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 19:04:12 by chaikney          #+#    #+#             */
/*   Updated: 2024/06/28 19:04:18 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Return the index of the first $ if command needs to
// have variable subsitution.
// If no sub is needed, return -1
// TODO Take into account "" and '' quoting to decide a sub is needed
int	needs_sub(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	if (str[i] == '$')
		return (i);
	else
		return (-1);
}

// Substitute one substring for another in a string
// DONE If new_sub or old_sub(?) are null, still have to get rid of the $
// (or we get an endless loop in substitute_variables)
// TODO Have to remove " and ' characters from the final cmd
// NOTE Does that get cut out of what we do here, or passed to the command to let that handle it¿???
// FIXME Adds a space for missing values, not needed? bash collapses it to a single space
// FIXME Too many lines in this function.
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
	while (*str != '$')	// copy until the $
		*new_str++ = *str++;
	if (new_sub)
	{
		while (*new_sub != '\0')
			*new_str++ = *new_sub++;
	}
	while ((*str != ' ') && (*str != '\0'))	// Here we step over the variable name in the command
		str++;
	if (*str == ' ')
		str++;		// NOTE tries to avoid the extra space issue; probably dangerous though
	while (*str != '\0')
		*new_str++ = *str++;
	return (cptr);
}

// Substitute a variable into its position in the command:
// - find position of a variable
// - read the name of that variable
// - fetch the value of the variable
// - put the value into the command
// - run again / recurse until we have no more things to sub
// NOTE cmd is assumed to be the unsplit input from readline
// FIXME Function has too many lines
// FIXED? Endless loop if replace or remove are null (Should remove the $ at least)
char	*substitute_variables(char *cmd)
{
	int		sub_pos;
	int		sub_len;
	char	*var_name;
	char	*val;
	char	*new_cmd;

	sub_pos = needs_sub(cmd);
	new_cmd = NULL;
	while (sub_pos != -1)	
	{
		sub_pos++;	// NOTE Start the count *after* the $
		sub_len = 0;
		while ((cmd[sub_pos + sub_len] != '\0')
			&& (cmd[sub_pos + sub_len] != ' '))
			sub_len++;
		var_name = ft_substr(cmd, sub_pos, (sub_len));
//		printf("searching for %s in env", var_name);
		val = getenv(var_name);
//		printf("\tfound: %s", val);
		new_cmd = ms_strsub(cmd, var_name, val);
		free(var_name);
		sub_pos = needs_sub(new_cmd);
		cmd = new_cmd;
	}
	return (cmd);
}
