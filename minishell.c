/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:47:55 by chaikney          #+#    #+#             */
/*   Updated: 2024/06/06 13:48:06 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_procstatus;

// Take a command (must be at argv[0])
// Check to see if it is itself a valid path.
// Otherwise, search for it in PATH
// Run the command using argv set -- assumed to be NT'd and valid
// NOTE Any fork-ing needed should have been handled before calling this.
// NOTE The assumptions here are: argv[0] is a command not a redirect.
// ...and that all arguments in cmd->argv can be managed by the command
// TODO Move this to a suitable other file.
// DONE define exit routines for not found and for exec failure.
// FIXED After one command has failed, we have to call EXIT twice to end.
// FIXED Too many lines in function
// NOTE The lines at the end are only reached if execve fails
void	run_command(t_command *cmd, char **envp)
{
	char	*prog;

	if (cmd->builtin != NONE)
	{
		executeBuiltin(cmd, envp);
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
	else if (execve(prog, cmd->argv, envp) == -1)
	{
		g_procstatus = errno;
		perror("Failed to execute program");
	}
	free (prog);
	exit_failed_pipe(cmd, -1, -1);
}

// Sends cmdline to parse function to get a t_command list
// Pass it to the complex function
// Clear the command struct on return.
// NOTE Was an execbuiltin check here
// as some builtins don't work in pipes!
// - EXIT has to be an exit from the shell.
// - EXPORT has to change values in the process above.
void	eval(char *cmdline, char **envp)
{
	t_command	*cmd;

	cmd = parse(cmdline);
	if (cmd)
	{
		direct_complex_command(cmd, envp);
		clear_t_command(cmd);
	}
}

// FIXME Not clear what cmdline == NULL attempts, i can't trigger it.
// ...readline man page says this what it returns on EOF on an empty line.
// TODO Ensure that *all* commands run quit or return to here.
// NOTE Exit called by user does not need to free prompt
// ....as we destory it before eval
int	main(int argc, char **argv, char **envp)
{
	char	*cmdline;
	char	*prompt;

	(void) argv;
	cmdline = NULL;
	if (argc == 1)
	{
		setup_signals();
		while (1)
		{
			prompt = get_prompt();
			cmdline = readline(prompt);
			free (prompt);
			if (cmdline == NULL)
				ms_exit(NULL);
			if ((cmdline[0] != '\0'))
			{
				add_history((const char *) cmdline);
				eval(cmdline, envp);
			}
		}
	}
	else
		printf("no args needed to minishell\n");
	return (0);
}
