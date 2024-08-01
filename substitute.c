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
// TODO Nothing calls this. Is it obsolete?
// TODO This should step over the name in the outer string
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

// Receives a string cmd and substitutes the value for any $VARIABLE
// - find position of a variable
// - read the name of that variable using ms_strsub and its length
// - fetch the value of the variable
// - put the value into the command using ms_strsub
// - run again / recurse until we have no more things to sub
// NOTE variable length and name do *not* include the $
char	*substitute_variables(char *cmd)
{
	int		s_pos;
	int		s_len;
	char	*var_name;
	char	*val;
	char	*new_cmd;

	s_pos = needs_sub(cmd);
	new_cmd = NULL;
	while (s_pos != -1)
	{
		s_pos++;
		s_len = 0;
		while ((cmd[s_pos + s_len] != '\0') && (cmd[s_pos + s_len] != ' '))
			s_len++;
		var_name = ft_substr(cmd, s_pos, (s_len));
		if (ft_strncmp(var_name, "?", 1) == 0)
			val = ft_itoa(g_procstatus);
		else
			val = getenv(var_name);
		new_cmd = ms_strsub(cmd, var_name, val);
		free(var_name);
		s_pos = needs_sub(new_cmd);
		cmd = new_cmd;
	}
	return (cmd);
}
