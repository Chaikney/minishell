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

int g_procstatus;

// Take a command (must be at argv[0])
// Check to see if it is itself a valid path.
// Otherwise, search for it in PATH
// Run the command using argv set -- assumed to be NT'd and valid
// NOTE Any fork-ing needed should have been handled before calling this.
// NOTE The assumptions here are: argv[0] is a command not a redirect.
// ...and that all arguments in cmd->argv can be managed by the command
// TODO Move this to a suitable other file.
// TODO define exit routines for not found and for exec failure.
void	run_command(t_command *cmd, char **envp)
{
	char	*prog;

	printf("about to run a command with %i args", cmd->argc);
	print_cmd_parts(cmd);	// HACK for debugging
//	printf("here to run a command: %s", cmd->argv[0]);	// HACK for debugging
	if (access(cmd->argv[0], X_OK) == 0)
		prog = cmd->argv[0];
	else
		prog = search_in_path(cmd->argv[0]);
	if (!prog)
	{
		g_procstatus = errno;
		perror("Program not found in PATH");
		free(prog);
//		exit_and_free(cmd, -1, -1);
	}
	if (execve(prog, cmd->argv, envp) == -1)	// if successful, execve does not return
	{
		g_procstatus = errno;
		perror("Failed to execute program");
        free (prog);
//		ms_exit(cmd);	// TODO Determine proper free-ing needs here.
//		exit_and_free(args, -1, -1);
	}
}

// Sends cmdline to parse function to get a t_command
// If the return has no control characters, send it to run.
// If there are control characters, pass it to the complex function
// Clear the command struct on return.
// DONE rename the bg variable to say what it does (what does it do?)
// NOTE First execbuiltin check looks to not be needed.
// BUT some builtins don't work in pipes!
// EXIT has to be an exit from the shell.
// EXPORT has to change values in the process above.
// Therefore we keep the check here!
void eval(char *cmdline, char **envp)
{
    int			parse_state;
    t_command	cmd;
	int			con_loc;

    parse_state = parse(cmdline, &cmd);
	if (parse_state == 0)
	{
		con_loc = find_flow_control(&cmd);
		if (con_loc == -1)
		{
			if (cmd.builtin != NONE)
				executeBuiltin(&cmd, envp);
			else
				run_final_cmd(&cmd, envp, -1, -1);
		}
		else
			direct_complex_command(&cmd, envp);
		clear_t_command(&cmd);
	}
}

// TODO Define a more interesting prompt, e.g. show wd.
// TODO Display exit status in prompt.
// TODO Add colours to prompt.
// This returns a text string to be dsiplayed by readline
// when waiting for user input
char	*get_prompt(void)
{
    return("what should i do? > ");
}

// FIXME Not clear what cmdline == NULL attempts, i can't trigger it.
// ...readline man page says this what it returns on EOF on an empty line.
// TODO Implement an exit routine that frees allocated memory.
// TODO Ensure that *all* commands run quit or return to here.
int main(int argc, char **argv, char **envp)
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
