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
	int		i;

	i = 0;
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
// DONE Adapt this to work with the minishell command struct
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


// DONE Calls to this to be replaced by calls to make_child / run_command above
// TODO Delete this later, for reference only.
void runSystemCommand(t_command *cmd, int bg)
{
    pid_t childPid;
    if ((childPid = fork()) < 0) = shell
        error("fork() error");
    else if (childPid == 0) 
    {
        if (execvp (cmd->argv[0], cmd->argv) < 0) 
        {
            printf("%s: Command not found\n", cmd->argv[0]);
            exit(0);
        }  
    }
    else 
    {
        if (bg)
            printf("Child in background [%d]\n", childPid);
        else
            wait(&childPid);
    }
}

// TODO Implement runBuiltinCommand
void eval(char *cmdline, char **envp) 
{
    int bg;
    t_command cmd;

    printf("Evaluating '%s'\n", cmdline);	// HACK For debugging, remove later
    bg = parse(cmdline, &cmd);
    printf("Found command %s\n", cmd.argv[0]);	// HACK For debugging, remove later
    if (bg == -1) 
        return;
    if (cmd.argv[0] == NULL) 
        return;
    if (cmd.builtin == NONE)
        make_child ( &cmd, bg, envp);
    else
        runBuiltinCommand( &cmd, bg);
}

// FIXED I think fgets is forbidden - we are supposed to use readline
// FIXME I think feof is forbidden - find another way to catch EOF signal.
// TODO Implement an exit routine that frees allocated memory.
// TODO cmdline must be freed after use.
// TODO Define a more interesting prompt
int main(int argc, char **argv, char **envp)
{
    char	*cmdline;
    char	*prompt;

    while (1) 
    {
        cmdline = readline(prompt);
        if (cmdline == NULL)
            perror("readline error");
        if (feof (stdin)) 
        {
            printf("\n");
            exit(0);
        }
        cmdline [ft_strlen(cmdline)-1] = '\0';
        eval (cmdline, envp);
        return (0);
    }
}
