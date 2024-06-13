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

// Take a command
// Find it in PATH
// Run the command
// NOTE Any fork-ing needed should have been handled before calling this.
// TODO Adapt this to work with the minishell command struct
// TODO We need a clear_command_struct function to free mem allocated to cmd
void	run_command(t_command *cmd, char **envp)
{
	char	*prog;

	prog = find_command(cmd->argv[0], envp);
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

// Make a child process to execute the command:
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// TODO We can make this work with & / background
// FIXME Some of this is only needed if we are pipe-ing, logic may be wrong.
void	make_child(t_command *cmd, int bg, char **envp)
{
	pid_t	child;
	int		tube[2];

	if (pipe(tube) == -1)
		exit_and_free(NULL, tube[0], tube[1]);
	child = fork();
	if (child == -1)
		exit_and_free(NULL, tube[0], tube[1]);
	if (child == 0)
	{
		close(tube[0]);
		dup2(tube[1], STDOUT_FILENO);
		run_command(cmd, envp);
		close(tube[1]);
	}
	else
	{
		close(tube[1]);
		dup2(tube[0], STDIN_FILENO);
		if (bg)
			printf("Child in background [%d]\n", child);
		else    // TODO Not sure about this - does the pipe work with bg? If we put a process in BG would we still redirect it?
			waitpid(child, 0, 0);
		close(tube[0]);
	}
}


/* // DONE Calls to this to be replaced by calls to make_child / run_command above */
/* // TODO Delete this later, for reference only. */
/* void runSystemCommand(t_command *cmd, int bg) */
/* { */
/*     pid_t childPid; */
/*     if ((childPid = fork()) < 0) = shell */
/*         error("fork() error"); */
/*     else if (childPid == 0)  */
/*     { */
/*         if (execvp (cmd->argv[0], cmd->argv) < 0)  */
/*         { */
/*             printf("%s: Command not found\n", cmd->argv[0]); */
/*             exit(0); */
/*         }   */
/*     } */
/*     else  */
/*     { */
/*         if (bg) */
/*             printf("Child in background [%d]\n", childPid); */
/*         else */
/*             wait(&childPid); */
/*     } */
/* } */

// DONE Implement runBuiltinCommand
// Sends cmdline to parse functtion to get a t_command
// If cmd is BUILTIN, run there
// Otherwise, try and run system command
// TODO Only execute builtins if the command is a builtin, else try and reun command
// TODO We also need to catch if running in a pipe or not.
// TODO Determine if there is input or output redirection.
// TODO Decide if run_command / cmd should be a pointer
// FIXME Commands need to be run through a child process.
void eval(char *cmdline, char **envp) 
{
    int			bg;
    t_command	cmd;

    printf("Evaluating '%s'\n", cmdline);	// HACK For debugging, remove later
    bg = parse(cmdline, &cmd);
    printf("Found command %s\n", cmd.argv[0]);	// HACK For debugging, remove later
    if (bg == -1) 
        return;
    if (cmd.argv[0] == NULL) 
        return;
    // FIXME if comented provisinaly becasuse this way the program works correctly when u input a wrong command
    /* if (cmd.builtin == NONE) */
    /*     make_child ( &cmd, bg, envp); */
    /* else */
	if (cmd.builtin != 0)
		executeBuiltin(&cmd, envp);
	else
		run_command(&cmd, envp);
}

/* void	add_history(char *line) */
/* { */
/*     (void) line; */
/*     printf("add_history not implmented yet"); */
/* } */

// TODO Define a more interesting prompt
char	*get_prompt(void)
{
    return("what should i do? > ");
}

// FIXME I think feof is forbidden - find another way to catch EOF signal.
// TODO Implement an exit routine that frees allocated memory.
// TODO cmdline must be freed after use.
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
				add_history(cmdline);
				eval(cmdline, envp);
			}
			free(cmdline);
		}
	}
	else
		printf("no args needed to minishell\n");
	return (0);
}
