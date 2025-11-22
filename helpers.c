/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:55:44 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/02 14:04:32 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Various finders and helper functions.

// return 1 if c is a flow control character, 0 if not
// Flow control chars are: < > |
int	is_control_char(char c)
{
	if ((c == '>') || (c == '<') || (c == '|'))
		return (1);
	else
		return (0);
}

// Moves the *posn pointer forward through a command line
// until it reaches a stop character (which is badly named because we
// use this to find a place to *start* parsing).
// The stop chars are null or any non-space character.
// (The quote styles are matched for in the main loop.)
void	goto_stop_char(const char *cmdline, int *posn)
{
	while ((cmdline[*posn] != '\0') && (((cmdline[*posn] == ' '))
			|| (cmdline[*posn] == '\n')
			|| (cmdline[*posn] == '\f')
			|| (cmdline[*posn] == '\v')
			|| (cmdline[*posn] == '\r')
			|| (cmdline[*posn] == '\t')))
		(*posn)++;
}

// Checks to see if the string is a valid shell variable name
// 1 - YES is a valid name
// 0 - NO the name is not valid
// Rules are:
// - allowed chars are alphanumeric and _ underscore
// - does not start with a digit
int	is_legal_name(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if ((ft_isdigit(str[0]) == 1) || (str[0] == '\'') || (str[0] == '\"'))
		return (0);
	if ((str[0] == '?') && (ft_strlen(str) == 1))
		return (1);
	while (str[i] != '\0')
	{
		if ((ft_isalnum(str[i]) == 1) || (str[i] == '_'))
			i++;
		else
			return (0);
	}
	return (1);
}

// return the VALUE part of a NAME=VALUE pair to process in EXPORT
// Variables:
// - value:		the string returned. Must be freed later.
// - midpoint:	index of the = character.
// - len:		Number of chars we have to copy from value.
char	*get_export_value(char *str)
{
	uint	midpoint;
	size_t	len;
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

// Return the NAME part of str. OR NULL if the name is invalid.
// NOTE Each argument to EXPORT should be
// in the form NAME=VALUE, without spaces
// therefore, the part to = is the NAME
// See how long the part before = is
// Allocate that space (+ 1 for null terminator)
// Copy backwards to zero
char	*get_export_name(char *str)
{
	size_t	len;
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
