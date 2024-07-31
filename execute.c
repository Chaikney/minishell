/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:37:50 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/31 19:46:30 by emedina-         ###   ########.fr       */
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
void	run_command(t_command *cmd, char **envp, t_env *envt)
{
	char	*prog;

	if (cmd->builtin != NONE)
	{
		execute_builtin(cmd, envt);
		exit_successful_pipe(cmd); 	// TODO Should a successful exit here reset g_procstatus?
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
	else if (execve(prog, cmd->argv, envp) == -1)
	{
		g_procstatus = errno;
		perror("Failed to execute program");
	}
	free (prog);
	exit_failed_pipe(cmd, -1, -1);
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
//  TODO switch EXPORT to work with t_env
//  TODO switch UNSET to work with t_env
//  TODO switch CD to work with t_env
void	execute_builtin(t_command *cmd, t_env *enviro)
{
	if (cmd->builtin == CD)
		ms_cd(cmd, enviro);
	else if (cmd->builtin == EXIT)
		ms_exit(cmd);
	else if ((cmd->builtin == ECHON) || (cmd->builtin == ECHO))
		ms_echo(cmd);
	else if (cmd->builtin == PWD)
		ms_pwd();
	else if (cmd->builtin == EXP)
		ms_export_t(cmd, &enviro);
//		ms_export(cmd, envp);
	else if (cmd->builtin == UNSET)
		ms_unset(cmd, enviro);
	else if (cmd->builtin == ENV)
		ms_env_t(enviro);
//		ms_env(envp);
	else
	{
		printf("Unknown builtin command\n");
		return ;
	}
}
