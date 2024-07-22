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

// this is to disable CRTL-D and the apparent EOF we get
// after a pipe ends
void	handle_sigquit()
{
	return ;
}

// catch CTRL-c / SIGINT
// TODO This should return to the normal prompt / readline call
// ...I think this leads to duplicated prompt message.
void manipule_sigint(int sig)
{
	if(sig)
	{
		printf("\nwhat should i do? >");
	}
}

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
	exit (EXIT_FAILURE);
}

// Free the memory allocated to a t_command and all those following it.
// Frees all argv, including the final NULL
void	clear_t_command(t_command *cmd)
{
	int	i;

	if (cmd)
	{
		while (cmd->next != NULL)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv[cmd->argc]);
			cmd = cmd->next;
		}
		i = 0;
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv[cmd->argc]);
	}
}

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
// TODO rename the bg variable to say what it does (what does it do?)
// TODO First execbuiltin check is not needed.
void eval(char *cmdline, char **envp)
{
    int			bg;
    t_command	cmd;
	int			con_loc;

    bg = parse(cmdline, &cmd);
	(void) bg;
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
	/* if (bg == -1)	// TODO Do we still need this, what is it for? */
	/* 	return; */
	clear_t_command(&cmd);
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
// TODO Implement signals handling SIGINT - needs fixed
// TODO Implement signals handling CTRL-D
// TODO Implement signals handling CTRL-\ uncontrolled quit
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
		signal(SIGQUIT, handle_sigquit);	// This works to supress ctrl-d but not the cmdline breakage
		while (1)
		{
			signal(SIGINT, manipule_sigint);
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
