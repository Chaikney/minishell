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

// Return the whole of the control sequence landed on.
// It will be either < << > >> or |
// And since we know where we started the question is do we take the
// first char only, or the second as well.
char	*grab_control_seq(const char *cmd, int *posn)
{
	int		i;
	char	*par;

	i = 0;
	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	if (cmd[*posn] == cmd[*posn + 1])
	{
			par[i] = cmd[*posn];
			i++;
			(*posn)++;
	}
	par[i] = cmd[*posn];
	i++;
	(*posn)++;
	par[i] = '\0';
	return (par);
}

// Return one single parameter from a command line.
// - allocate arbitrary space
// - if we are at the end, reset the static and return NULL to finish.
// - step over initial spaces, avoiding the end marker
// - copy characters
// - null-term and return
// DONE handle some escape characters in this function: if \, copy the next char
// TODO Consider if this should stop at a ' or "
// (they would be misplaced, but...)
// NOTE Fish and bash react to echo 'thing but opening multiline input.
// ....really don't see a way to achieving that.
// NOTE It is *only* this function which loses memory. What is the difference?
// DONE Handle case when we have > < without spaces to the next
// DONE Handle command *ending* with a control char, i.e. Test ls> test
char	*get_raw_param(const char *cmd, int *posn)
{
	int		i;
	char	*par;

	// HACK These two lines debugging only, can remove later.
	if ((cmd[*posn] == ' ') || (cmd[*posn] == '\0'))
		printf("\t*** entered at incorrect char: %c", cmd[*posn]);
	// have we started at a control char?
	if (is_control_char(cmd[*posn]) == 1)
		par = grab_control_seq(cmd, posn);
	// this is the part that copies.
	else
	{
		par = malloc(sizeof(char) * 256);
		if (!par)
			return (NULL);
		ft_bzero(par, 256);
		i = 0;
		while ((cmd[*posn] != '\0') && (cmd[*posn] != ' ') && (is_control_char(cmd[*posn]) == 0))
		{
			if (cmd[*posn] == '\\')
			{
				(*posn)++;
				// copy the next char
			}
			par[i] = cmd[*posn];
			i++;
			(*posn)++;
		}
		par[i] = '\0';
	}
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
