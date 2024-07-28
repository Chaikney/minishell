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
// TODO There are too many functions in helpers.c Find other places for them!

// Returns the line index number of PATH for the requested var string
// If not found, returns -1
int	find_env_var(char **envp, const char *var)
{
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	len = ft_strlen(var);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (i);
		else if(ft_strncmp(envp[i], var, len) == 0 && envp[i][len] != '=')
			return(i);
		i++;
	}
	return (-1);
}

// return 1 if c is a flow control character, 0 if not
// Flow control chars are: < > |
int	is_control_char(char c)
{
	if ((c == '>') || (c == '<') || (c == '|'))
		return (1);
	else
		return (0);
}

// Look through a parsed set of commands and see if it contains
// flow control parameters: | > < >> or <<
// They would be separate from commands either side.
// (locates, it is up to the outer loop to re-identify what the thing is)
// FIXME Segfaults if cmdset (or any part of it) is null, add protection
// TODO find_flow_control could be removed now, or find other use of it.
int	find_flow_control(t_command *cmdset)
{
	int	i;

	i = 0;
	while (cmdset->argv[i])
	{
		if (ft_strncmp(cmdset->argv[i], ">>", 2) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], "<<", 2) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], ">", 1) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], "<", 1) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], "|", 1) == 0)
			break ;
		i++;
	}
	if (i == cmdset->argc)
		i = -1;
	return (i);
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
char	*search_in_path(char *cmd)
{
	char	**pathparts;
	char	*candidate;
	char	*slashed;
	char	*goodpath;
	int		i;

	i = 0;
	goodpath = NULL;
	pathparts = ft_split(getenv("PATH"), ':');
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
