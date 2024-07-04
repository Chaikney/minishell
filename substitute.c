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

// Return the index of the first $ if the string needs to
// have variable subsitution.
// If no sub is needed, return -1
// NOTE Attempts to ignore values within 'strong quoting'.
int	needs_sub(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			break ;
		if (str[i] == '\'')
		{
			while ((str[i] != '\0') && (str[i] != '\''))
				i++;
		}
		if (str[i] != '\0')
			i++;
	}
	if (str[i] == '$')
		return (i);
	else
		return (-1);
}

// Return the name of a $variable for later substitution
// Returns a null-terminated string *without* the $
// - check we are at $
// - count length (until a space)
// - alloc a string
// - copy characters
// FIXME Function too long. Can I do this with fewer vars?
char	*get_var_name(const char *str)
{
	int		name_len;
	char	*ptr;
	char	*var_name;

	ptr = (char *) str;
	if (*ptr != '$')
	{
		printf("Tried to find a variable but you passed the wrong position");
		return (NULL);
	}
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
// KILL out of scope here Have to remove " and ' characters from the final cmd
// NOTE Does that get cut out of what we do here, or passed to the command to let that handle it¿???
// NOTE Adds a space for missing values, not needed? bash collapses it to a single space
// FIXED Too many lines in this function.
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

// Acts on the input as a whole:
// Substitute a variable into its position in the command:
// - find position of a variable
// - read the name of that variable
// - fetch the value of the variable
// - put the value into the command
// - run again / recurse until we have no more things to sub
// NOTE cmd is assumed to be the unsplit input from readline
// FIXME Function has too many lines
// TODO Also have to handle $? / ? as a var name, shows the exit status.
// TODO Not sure if this is even useful any more.
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
//		var_name = get_var_name(&cmd[sub_pos]);	// FIXME This should work but segfaults.
//		printf("searching for %s in env", var_name);	// HACK for debugging
		val = getenv(var_name);
//		printf("\tfound: %s", val);	// HACK for debugging
		new_cmd = ms_strsub(cmd, var_name, val);
		free(var_name);
		sub_pos = needs_sub(new_cmd);
		cmd = new_cmd;
	}
	return (cmd);
}
