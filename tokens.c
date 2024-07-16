/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:56:44 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/16 17:56:47 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Functions used to break up the readline input into tokens for parsing.

// Return one single parameter from a command line.
// - allocate arbitrary space
// - if we are at the end, reset the static and return NULL to finish.
// - step over initial spaces, avoiding the end marker
// - copy characters
// - null-term and return
// TODO We may need to handle some escape characters in this function.
// TODO Consider if this should stop at a ' or "
// (they would be misplaced, but...)
char	*get_raw_param(const char *cmd, int *posn)
{
	int		i;
	char	*par;

	if ((cmd[*posn] == ' ') || (cmd[*posn] == '\0'))
		printf("\t*** entered at incorrect char: %c", cmd[*posn]);
	i = 0;
	par = malloc(sizeof(char) * 256);	// FIXME block lost after any redirection.
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	while ((cmd[*posn] != '\0') && (cmd[*posn] != ' '))
	{
		par[i] = cmd[*posn];
		i++;
		(*posn)++;
	}
	par[i] = '\0';
	return (par);
}

// Return a "weakly-quoted" parameter from cmdline.
// (Weakly quoted = parameter expansion happens, space separation doesn't)
// - Find the first double quote, step past it
// - Start copying characters until the end quote
// - Step past the end quote if that will not go out of bounds.
// TODO Should this null-terminate its return value?
char	*get_weak_param(const char *cmdline, int *posn)
{
	char	*par;
	int		j;

	if (cmdline[*posn] != '\"')
		printf("\t*** entered at incorrect char: %c", cmdline[*posn]);
	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	j = 0;
	(*posn)++;
	while ((cmdline[*posn] != '\"') && (cmdline[*posn] != '\0'))
	{
		par[j] = cmdline[*posn];
		j++;
		(*posn)++;
	}
	if (cmdline[*posn] != '\0')
		(*posn)++;
	return (par);
}

// return a 'strongly-quoted' parameter from cmdline.
// (Strongly quoted = no escaping or substituting).
// - Find the first single quote, step past it.
// - start copying characters until the next quote
// - step past the final ' if safe
// NOTE This version does not allow for escaped quotes!
// TODO Should this null-terminate its return value?
char	*get_strong_param(const char *cmdline, int *posn)
{
	char	*par;
	int		j;

	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	j = 0;
	if (cmdline[*posn] != '\'')
		printf("\t*** entered at incorrect char: %c", cmdline[*posn]);
	(*posn)++;
	while ((cmdline[*posn] != '\'') && (cmdline[*posn] != '\0'))
	{
		par[j] = cmdline[*posn];
		j++;
		(*posn)++;
	}
	if (cmdline[*posn] != '\0')
		(*posn)++;
	return (par);
}
