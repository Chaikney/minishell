/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:55:44 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/16 17:55:49 by chaikney         ###   ########.fr       */
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
	while ((cmdline[*posn] != '\0') && (cmdline[*posn] == ' '))
		(*posn)++;
}

// Return an executable path for cmd.
// NOTE cmd must be the name only, not any of its arguments.
// - split the pieces of PATH and add a trailing slash.
// - test the parts of path:
// -- does path + cmd = an executable?
// -- if YES we have our command: keep that and discard the rest.
// NOTE The return value of getenv("PATH") does not need to be freed
// FIXED Don't use getenv for this.
// FIXME search_in_path is too long for norm
char	*search_in_path(char *cmd, t_env *envt)
{
	char	**pathparts;
	char	*candidate;
	char	*slashed;
	char	*goodpath;
	int		i;

	i = 0;
	goodpath = NULL;
	pathparts = ft_split(get_value_of_env("PATH", envt), ':');
	while ((pathparts[i] != NULL) && (!goodpath))
	{
		slashed = ft_strjoin(pathparts[i], "/");
		candidate = ft_strjoin(slashed, cmd);
		if (access(candidate, X_OK) == 0)
			goodpath = ft_strdup(candidate);
		free (candidate);
		free(slashed);
		i++;
	}
	i = -1;
	while (pathparts[++i] != NULL)
		free(pathparts[i]);
	free(pathparts);
	return (goodpath);
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
	if (ft_isdigit(str[0]) == 1)
		return (0);
	while (str[i] != '\0')
	{
		if ((ft_isalnum(str[i]) == 1) || (str[i] == '_'))
			i++;
		else
			return (0);
	}
	return (1);
}
