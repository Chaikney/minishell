/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:37:50 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/01 02:58:50 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Functions that handle the execution and running of commands.

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
// TODO Need to serialise_envt so that execve gets uptodate ENV
void	run_command(t_command *cmd, char **envp, t_env *envt)
{
	char	*prog;

	(void) envp;
	if (cmd->builtin != NONE)
	{
		execute_builtin(cmd, envt);
		exit_successful_pipe(cmd);
	}
	if (access(cmd->argv[0], X_OK) == 0)
		prog = cmd->argv[0];
	else
		prog = search_in_path(cmd->argv[0]);
	if ((!prog) || (access(prog, X_OK) != 0))
	{
		g_procstatus = errno;
		perror("Executable program not found in PATH");
	}
	else if (execve(prog, cmd->argv, serialise_env(envt)) == -1)
	{
		g_procstatus = errno;
		perror("Failed to execute program");
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
