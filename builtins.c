/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:47:34 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/07 23:31:45 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Some shell builtin functions, may split later.

// The more complicated / slower parsing that we need to do to identify
// a builtin.
static t_builtin	detailed_parse_builtin(t_command *cmd, int posn)
{
	int	len;

	len = ft_strlen(cmd->argv[posn]);
	if ((ft_strncmp(cmd->argv[posn], "exit", 4) == 0) && (len == 4))
		return (EXIT);
	else if ((ft_strncmp(cmd->argv[posn], "unset", 5) == 0) && (len == 5))
		return (UNSET);
	else if ((ft_strncmp(cmd->argv[posn], "export", 6) == 0) && (len == 6))
		return (EXP);
	else if ((ft_strncmp(cmd->argv[posn], "echo", 4) == 0) && (len == 4))
	{
		if ((cmd->argv[posn + 1]) && (ft_strlen(cmd->argv[posn + 1]) == 2)
			&& (ft_strncmp(cmd->argv[posn + 1], "-n", 2) == 0))
			return (ECHON);
		else
			return (ECHO);
	}
	return (NONE);
}

// Read the given command argument and if it matches a builtin, set the flag.
// (posn will be 0 except when there is input redirection)
// First a sanity check on argument annd position,
// then we check whether the first letter matches one of the builtin names.
// then the shorter names.
// If more complicated parsing is needed, we call detailed_parse_builtin
// NOTE We no longer check if the command has extra arguments
// - the builtin can just ignore them
// FIXED Segfaults here given ><
// It splits to 2 tokens but one command - should not happen?
// but how to guard against?
// FIXED Still a problem if >© is given.
t_builtin	parse_builtin(t_command *cmd, int posn)
{
	t_builtin	retvalue;
	int			len;

	if ((cmd->argc == 0) || (cmd->argc < posn))
		retvalue = (NONE);
	else
	{
		len = ft_strlen(cmd->argv[posn]);
		if (ft_strchr("cepu", cmd->argv[posn][0]) == NULL)
			retvalue = (NONE);
		else if ((ft_strncmp(cmd->argv[posn], "cd", 2) == 0) && (len == 2))
			retvalue = (CD);
		else if ((ft_strncmp(cmd->argv[posn], "pwd", 3) == 0) && (len == 3))
			retvalue = PWD;
		else if ((ft_strncmp(cmd->argv[posn], "env", 3) == 0) && (len == 3))
			retvalue = ENV;
		else
			retvalue = detailed_parse_builtin(cmd, posn);
	}
	return (retvalue);
}

// Minishell builtin, our version of pwd.
// Calls getcwd, displays wd then frees the variable.
// NOTE Calling getcwd with NULL and 0 mallocs as much space as is needed
int	ms_pwd(void)
{
	char	*wd;

	wd = NULL;
	wd = getcwd(wd, 0);
	if (!wd)
		return (-1);
	printf("%s\n", wd);
	free (wd);
	g_procstatus = 0;
	return (0);
}

// return 1 if the command needs to fork before running
// Mostly important for builtins that should allow their output to redirect.
int	needs_to_fork(t_command *cmd)
{
	int	to_fork;

	to_fork = 0;
	if ((cmd->builtin == NONE) || (cmd->builtin == ENV) || (cmd->builtin == PWD)
		|| (cmd->builtin == ECHO) || (cmd->builtin == ECHON))
		to_fork = 1;
	else if ((cmd->builtin == EXP) && (cmd->argc == 1))
		to_fork = 1;
	return (to_fork);
}
