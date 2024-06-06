/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:03:41 by chaikney          #+#    #+#             */
/*   Updated: 2024/05/31 16:03:42 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Find the line in env that has PWD and return it.
// Works similar to get_path
// Returns a pointer to the location where the var begins. We would have to read
// onwards to a new line when pwd is called.
// ...would an allocated var be better?
// TODO Implement a function that prints this to the output
char	*get_pwd(char **envp)
{
	while (*envp != NULL)
	{
		if (ft_strncmp(*envp, "PWD=", 4) == 0)
			break ;
		envp++;
	}
	return (*envp + 4);
}

// Find the line in env that has PATH and return it split.
char	**get_path(char **envp)
{
	while (*envp != NULL)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			break ;
		envp++;
	}
	return (ft_split(*envp + 5, ':'));
}

// Locate PATH and go through entries for cmd is locatable.
// cmd must be the name only, not any of its arguments.
// - Find the PATH= line in env (this could be split out)
// - split the pieces of PATH and add a trailing slash.
// - (this also requires the first 5 chars of the line to be removed.)
// - test the parts of path:
// -- does path + cmd = an executable?
// -- if YES we have our command: keep that and discard the rest.
char	*find_command(char *cmd, char **envp)
{
	char	**pathparts;
	char	*candidate;
	char	*slashed;
	char	*goodpath;
	int		i;

	i = 0;
	goodpath = NULL;
	pathparts = get_path(envp);
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
