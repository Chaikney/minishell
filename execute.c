/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:37:50 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/07 22:03:10 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Functions that handle the execution and running of commands.

// Not S_ISREG - Not a regular file, don't execute
// Not S_IXUSER - Not executable by owner, don't execute
int	check_prog(char *prog)
{
	struct stat	pstat;
	int			retval;

	retval = 0;
	if ((!prog) || (stat(prog, &pstat) == -1))
	{
		g_procstatus = errno;
		perror("not found or cannot stat program");
		retval = -1;
	}
	else if (!S_ISREG(pstat.st_mode))
	{
		g_procstatus = EPERM;
		perror("Not a regular file");
		retval = -1;
	}
	else if (!(pstat.st_mode & S_IXUSR))
	{
		g_procstatus = EPERM;
		perror("Not executable");
		retval = -1;
	}
	return (retval);
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

// Take a command (must be at argv[0])
// If it is a builtin, run that and exit the child process.
// NOTE Ensure that EXIT has been handled outside of a fork!
// Check to see if argv[0] is itself a valid executable
// Otherwise, search for it in PATH
// NOTE The assumptions here are:
// - argv[0] is a command not a redirect.
// - all arguments in cmd->argv can be managed by the command
// - i.e. they are valid and null-terminated.
// - Any fork-ing needed has been handled before calling this.
// NOTE The lines at the end are only reached if execve fails
// DONE Need to serialise_envt so that execve gets uptodate ENV
// FIXED Block attempts to exectute directories (. or ...)
void	run_command(t_command *cmd, t_env *envt)
{
	char	*prog;

	prog = NULL;
	if (cmd->builtin != NONE)
	{
		execute_builtin(cmd, envt);
		exit_successful_pipe(cmd);
	}
	if (access(cmd->argv[0], X_OK) == 0)
		prog = ft_strdup(cmd->argv[0]);
	else if (is_in_envt("PATH", envt) == 1)
		prog = search_in_path(cmd->argv[0], envt);
	if (check_prog (prog) == 0)
	{
		if (execve(prog, cmd->argv, serialise_env(envt)) == -1)
		{
			g_procstatus = errno;
			perror("Failed to execute program");
		}
	}
	free (prog);
	exit_failed_pipe(cmd, -1, -1, envt);
}

// Check value of cmd->builtin and direct to desired function.
// falta por añadir que hace cada
//  [x] echo with -n (newline or not)
//  [x] cd with only a relative or absolute path
//  [x] pwd (no options)
//  [x] unset (no options)
//  [x] env, no options or args
//  [x] export
//  [x] exit no options.
//  NOTE Some of the builtins must redirect their output
//  - env
//  - export
//  - pwd
//  - echo
//  And some that must *not* redirect output
//  - unset
//  - cd
//  - export with value
//  - exit
//  This is handled in needs_to_fork and is transparent to the program
void	execute_builtin(t_command *cmd, t_env *enviro)
{
	if (cmd->builtin == CD)
		ms_cd(cmd, enviro);
	else if (cmd->builtin == EXIT)
		ms_exit(cmd, enviro);
	else if ((cmd->builtin == ECHON) || (cmd->builtin == ECHO))
		ms_echo(cmd);
	else if (cmd->builtin == PWD)
		ms_pwd();
	else if (cmd->builtin == EXP)
		ms_export_t(cmd, &enviro);
	else if (cmd->builtin == UNSET)
		ms_unset(cmd, enviro);
	else if (cmd->builtin == ENV)
		ms_env_t(enviro);
	else
	{
		printf("Unknown builtin command\n");
		return ;
	}
}
