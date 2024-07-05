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

void manipule_sigint(int sig)
{
	if(sig)
	{
		printf("\nwhat should i do? >");
	}
}
// TODO Implement a shell :-)

// Clear the pathparts and close fds created by piping before exit.
// TODO This probably needs to be expanded and / or replaced for minishell use.
// TODO Should this be merged into or called by ms_exit?
void	exit_and_free(char **args, int fd_in, int fd_out)
{
	int	i;

	perror(strerror(errno));
	if ((fd_in) && (fd_in != -1))
		close(fd_in);
	if ((fd_out) && (fd_out != -1))
		close(fd_out);
	i = 0;
	if (args)
	{
		while (args[i])
			free(args[i++]);
	}
	exit(EXIT_FAILURE);
}

// Free the memory allocated to a t_command
// Mainly (only?) in the args part.
// TODO Ensure that this is protected against empty cmds
void	clear_t_command(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[i])
			free(cmd->argv[i++]);
}

// Take a command (currently assumed argv[0])
// Check to see if it is itself a valid path.
// Otherwise, find it in PATH
// Run the command using argv set -- assumed to be NT'd and valid
// NOTE Any fork-ing needed should have been handled before calling this.
// NOTE The assumptions here are: argv[0] is a command not a redirect.
// ...and that we can pass the arguments using cmd->argv
// - this will fail when there are control characters and or multiple parameters
// TODO Check we have a valid format of argv (NTd, only one command and parameters)
void	run_command(t_command *cmd, char **envp)
{
	char	*prog;

	if (access(cmd->argv[0], X_OK) == 0)
		prog = cmd->argv[0];
	else
		prog = search_in_path(cmd->argv[0]);
	if (!prog)
	{
		perror("Program not found in PATH");
		free(prog);
//		exit_and_free(cmd, -1, -1);
	}
	if (execve(prog, cmd->argv, envp) == -1)
	{
		perror("Failed to execute program");
        free (prog);
		
//		exit_and_free(args, -1, -1);
	}
}

// Sends cmdline to parse functtion to get a t_command
// If cmd is BUILTIN, run there
// Otherwise, try and run a system command
// TODO We also need to catch if running in a pipe or not.
// TODO Determine if there is input or output redirection.
// TODO Decide if run_command / cmd should be a pointer
// TODO rename the bg variable to say what it does (what does it do?)
void eval(char *cmdline, char **envp)
{
    int			bg;
    t_command	cmd;

//    printf("Evaluating '%s'\n", cmdline);	// HACK For debugging, remove later
    bg = parse(cmdline, &cmd);
//    printf("Found command %s\n", cmd.argv[0]);	// HACK For debugging, remove later
    if (bg == -1) 
	{
        return;
	}
    if (cmd.argv[0] == NULL) 
	{
        return;
	}
	if (cmd.builtin != NONE)	// FIXME How does this work now with the enum changes?
		executeBuiltin(&cmd, envp);
	else
	{
		run_in_child(&cmd, envp);
	}
	clear_t_command(&cmd);
}

// TODO Define a more interesting prompt, e.g. show wd.
char	*get_prompt(void)
{
    return("what should i do? > ");
}

// NOTE feof is forbidden - find another way to catch EOF signal.
// FIXME Not clear what cmdline == NULL attempts, i can't trigger it.
// ...readline man page says this what it returns on EOF on an empty line.
// TODO Implement signals handling.
// TODO Implement an exit routine that frees allocated memory.
// DONE? Add cmdline to readline history after we receive it.
// TODO Implement a global variable to handle process status.
int main(int argc, char **argv, char **envp)
{
	char	*cmdline;
	char	*prompt;

	(void) argv;
	if (argc == 1)	// HACK for compilation, remove later
	{
		while (1)
		{
			g_procstatus = 0;
			signal(SIGINT, manipule_sigint);
			prompt = get_prompt();
			cmdline = readline(prompt);
			if (cmdline == NULL)
			{
				printf("\n");
				exit(0);
			}
			if (cmdline[0] != '\0')
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
