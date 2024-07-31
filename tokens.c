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

// Initialiase a string to copy the parts of the line to.
// NOTE 256 is abitary and might better as compile-time constant.
char	*get_blank_param(void)
{
	char	*par;

	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	return (par);
}

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
// NOTE Fish and bash react to echo 'thing by opening multiline input.
// But we do not have to handle unclosed quotes, says the subject PDF.
char	*get_raw_param(const char *cmd, int *posn)
{
	int		i;
	char	*par;

	// have we started at a control char?
	if (is_control_char(cmd[*posn]) == 1)
		par = grab_control_seq(cmd, posn);
	// this is the part that copies.
	else
	{
		par = get_blank_param();
		i = 0;
		while ((cmd[*posn] != '\0') && (cmd[*posn] != ' ') && (is_control_char(cmd[*posn]) == 0))
		{
			if (cmd[*posn] == '\\')
			{
				(*posn)++;
				// copy the next char
			}
			if (cmd[*posn] == '\"')
			{
				(*posn)++;
				// copy until next one
				while ((cmd[*posn] != '\"') && (cmd[*posn] != '\0'))
					par[i++] = cmd[(*posn)++];
			}
			if (cmd[*posn] != '\"')
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
// NOTE bzero call means we don't need to explicitly null-terminate par.
// FIXED export MS_TEST="can have spaces" is being split and should not be.
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
// NOTE bzero call means we don't need to explicitly null-terminate par.
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

// Find and remove (freeing memory) target string from cmd argument list.
// Target can match on a single character
// Target will be a control character (< > |)
// < << > and >> are associated with the parameter after,
// ...so we also remove that.
// ...controlled with the to_go variable.
// Method:
// - Decide if one or two entries must be removed.
// - Find position of the target
// - free memory at position (and next if needed)
// - copy the values ahead to the position
// - Ensure last NULL is copied
// - Adjust value of argc
// NOTE The target parts *must* have already been processed.
void	remove_cmd_parts(t_command *cmd, char *target)
{
	int	i;
	int	to_go;

	i = 0;
	if (target[0] == '|')
		to_go = 1;
	else
		to_go = 2;
	while ((cmd->argv[i]) && (ft_strncmp(cmd->argv[i], target, 1) != 0))
	{
		if (i == cmd->argc - 1)
			return ;
		i++;
	}
	free (cmd->argv[i]);
	if (to_go == 2)
		free (cmd->argv[i + 1]);
	while (cmd->argv[i + to_go] != NULL)
	{
		cmd->argv[i] = cmd->argv[i + to_go];
		i++;
	}
	cmd->argv[i] = cmd->argv[i + to_go];
	cmd->argc = cmd->argc - to_go;
}

// When we reach a quote, work out what the change is.
// What takes priority and what is right behaviour?
// Either way, we step past the character after processing it.
void	change_parse_mode(char c, int *mode, int *pos)
{
	if ((c == '\'') && (*mode != 2))
		*mode = 2;
	else if ((c == '\'') && (*mode == 2))
		*mode = 0;
	else if ((c == '\"') && (*mode != 1))
		*mode = 1;
	else if ((c == '\"') && (*mode == 1))
		*mode = 0;
	(*pos)++;
}

// Find a variable and add its value into the parameter.
// FIXME Does not retrieve custom-set variables.
int	add_value_into_param(char **par, int *r_posn, const char *cmdline)
{
	char	*vname;
	char	*vvalue;
	int		name_len;
	int		val_len;

	vname = get_var_name(&cmdline[*r_posn]);	// FIXME But probably not quote aware!
	val_len = 0;
	printf("subsituing %s", vname);
	if (vname)
	{
		name_len = ft_strlen(vname) + 1;
		vvalue = getenv(vname);
		while (name_len-- > 0)
			(*r_posn)++;
		printf("subsituing %s", vvalue);
		if (vvalue)
		{
			val_len = ft_strlen(vvalue);
			while (*vvalue != '\0')
			{
				printf("\ncopying %c\t", *vvalue);
				*(*par)++ = *vvalue++;
			}
		}
		free (vname);
	}
	return (val_len);
}

// p_mode (parsing style)
// 0 - raw
// 1 - "weak"
// 2 - 'strong'		copy everything in the quotes, unaltered
// FIXED Every cmd line with " or ' gives a blank parameter (or a space?)
// FIXED echo '$HOME' is an instant segfault. Reaches add_value_into_param and should not.
// FIXED echo one'with space' gives output of with space (loses the chars before ')
// FIXED raw quote does not engage variable sub
// FIXED variable sub is just generally WEIRD the pointer don't move.
char	*get_any_parameter(const char *cmdline, int *posn)
{
	char	*param;
	char	*ptr;
	int		p_mode;
	char	stops[8] = "|><$ \'\"\\";

	p_mode = 0;
	param = get_blank_param();
	ptr = param;
	while (cmdline[*posn] != '\0')
	{
		if ((p_mode == 0) &&
			((is_control_char(cmdline[*posn]) == 1) || ((p_mode == 0) && (cmdline[*posn]) == ' ')))
			break ;
		while ((p_mode == 2) && (ft_strchr("\''\0", cmdline[*posn]) == NULL))
			*ptr++ = cmdline[(*posn)++];
		while ((p_mode == 1) && (ft_strchr("\"$", cmdline[*posn]) == NULL))
			*ptr++ = cmdline[(*posn)++];
		while ((p_mode == 0) && (ft_strchr(stops, cmdline[*posn]) == NULL))
			*ptr++ = cmdline[(*posn)++];
		if ((p_mode != 2) && (cmdline[*posn] == '$'))
			add_value_into_param(&ptr, posn, cmdline);
		if ((cmdline[*posn] == '\'') || (cmdline[*posn] == '\"'))
			change_parse_mode(cmdline[*posn], &p_mode, posn);
	}
	return (param);
}
