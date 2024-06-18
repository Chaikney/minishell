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

// TODO Implement a shell :-)

// Clear the pathparts and close fds created by piping before exit.
// TODO This probably needs to be expanded and / or replaced for minishell use.
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
void	clear_t_command(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[i])
			free(cmd->argv[i++]);
}

// Take a command
// Find it in PATH
// Run the command
// NOTE Any fork-ing needed should have been handled before calling this.
// DONE in the basic Adapt this to work with the minishell command struct
// DONE We need a clear_command_struct function to free mem allocated to cmd
void	run_command(t_command *cmd, char **envp)
{
	char	*prog;

	prog = find_command(cmd->argv[0]);
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
// Otherwise, try and run system command
// DONE Only execute builtins if the command is a builtin, else try and reun command
// TODO We also need to catch if running in a pipe or not.
// TODO Determine if there is input or output redirection.
// TODO Decide if run_command / cmd should be a pointer
// FIXED Commands need to be run through a child process.
void eval(char *cmdline, char **envp) 
{
    int			bg;
    t_command	cmd;

//    printf("Evaluating '%s'\n", cmdline);	// HACK For debugging, remove later
    bg = parse(cmdline, &cmd);
//    printf("Found command %s\n", cmd.argv[0]);	// HACK For debugging, remove later
    if (bg == -1) 
        return;
    if (cmd.argv[0] == NULL) 
        return;
	if (cmd.builtin != 0)
		executeBuiltin(&cmd, envp);
	else
	{
		run_in_child(&cmd, envp);
	}
	clear_t_command(&cmd);
}

// TODO Define a more interesting prompt
char	*get_prompt(void)
{
    return("what should i do? > ");
}

// FIXME feof is forbidden - find another way to catch EOF signal.
// TODO Implement signals handling.
// TODO Implement an exit routine that frees allocated memory.
// DONE cmdline must be freed after use.
// DONE Add cmdline to readline history after we receive it.
// TODO Persist history between runs of minishell.
int main(int argc, char **argv, char **envp)
{
	char	*cmdline;
	char	*prompt;

	(void) argv;
	if (argc == 1)	// HACK for compilation, remove later
	{
		while (1)
		{
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
			free(cmdline);
		}
	}
	else
		printf("no args needed to minishell\n");
	return (0);
}
