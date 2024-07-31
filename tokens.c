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
// FIXME function too long (check without printfs)
// TODO Plug the ENVIRONMENT into this to get values from it
int	add_value_into_param(char **par, int *r_posn, const char *cmdline, t_env *envt)
{
	char	*vname;
	char	*vvalue;
	int		name_len;
	int		val_len;

	vname = get_var_name(&cmdline[*r_posn]);
	val_len = 0;
	printf("subsituing %s", vname);
	if (vname)
	{
		name_len = ft_strlen(vname) + 1;
		vvalue = get_value_of_env(vname, envt);
		while (name_len-- > 0)
			(*r_posn)++;
		printf("subsituing %s", vvalue);
		if (vvalue)
		{
			val_len = ft_strlen(vvalue);
			while (*vvalue != '\0')
				*(*par)++ = *vvalue++;
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
char	*get_any_parameter(const char *cmdline, int *posn, t_env *envt)
{
	char	*param;
	char	*ptr;
	int		p_mode;

	p_mode = 0;
	param = get_blank_param();
	ptr = param;
	while (cmdline[*posn] != '\0')
	{
		if ((p_mode == 0) && ((is_control_char(cmdline[*posn]) == 1)
				|| ((p_mode == 0) && (cmdline[*posn]) == ' ')))
			break ;
		while ((p_mode == 2) && (ft_strchr("\''\0", cmdline[*posn]) == NULL))
			*ptr++ = cmdline[(*posn)++];
		while ((p_mode == 1) && (ft_strchr("\"$", cmdline[*posn]) == NULL))
			*ptr++ = cmdline[(*posn)++];
		while ((p_mode == 0) && (!ft_strchr("|><$ \'\"", cmdline[*posn])))
			*ptr++ = cmdline[(*posn)++];
		if ((p_mode != 2) && (cmdline[*posn] == '$'))
			add_value_into_param(&ptr, posn, cmdline, envt);
		if ((cmdline[*posn] == '\'') || (cmdline[*posn] == '\"'))
			change_parse_mode(cmdline[*posn], &p_mode, posn);
	}
	return (param);
}
