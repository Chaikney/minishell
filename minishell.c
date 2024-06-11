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

// Clear the pathparts and close fds before exit.
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
void	run_command(struct command *cmd, char **envp)
{
	char	*prog;
	char	**args;
	int		i;

	args = ft_split(cmd, ' ');  // TODO Change/remove this, as we have already parsed the command
	i = 0;
	prog = find_command(args[0], envp);
	if (!prog)
	{
		perror("Program not found in PATH");
		free(prog);
		exit_and_free(args, -1, -1);
	}
	if (execve(prog, args, envp) == -1)
	{
		perror("Failed to execute program");
		exit_and_free(args, -1, -1);
	}
	if (args)   // NOTE If the execve succeeds, this will not run.
		while (args[i])
			free(args[i++]);
	free (prog);
}

// Make a child process to execute the commnand:
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// TODO Adapt this to work with the minishell command struct
// TODO We can make this work with & / background
void	make_child(struct command *cmd, int bg, char **envp)
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
        if (bg)
            printf("Child in background [%d]\n", child);
        else    // TODO Not sure about this - does the pipe work with bg? If we put a process in BG would we still redirect it?
        {
		    dup2(tube[0], STDIN_FILENO);
		    waitpid(child, 0, 0);
        }
		close(tube[0]);
	}
}


// TODO Calls top this to be replaced by calls to make_child / run_command above
void runSystemCommand(struct command *cmd, int bg) 
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

// TODO Implement runBuiltinCOmmand
void eval(char *cmdline, char **envp) 
{
    int bg;
    struct command cmd;
    printf("Evaluating '%s'\n", cmdline);
    bg = parse(cmdline, &cmd);
    printf("Found command %s\n", cmd.argv[0]);
    if (bg == -1) 
        return;
    if (cmd.argv[0] == NULL) 
        return;
    if (cmd.builtin == NONE)
        make_child ( &cmd, bg, envp);
    else
        runBuiltinCommand( &cmd, bg);
}

// FIXME I think fgets is forbidden - we are supposed to use readline
/* int main(int argc, char **argv, char **envp) {
    char cmdline [MAXLINE];
    while (1) 
    {
        printf("%s", prompt);
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
        error("fgets error");
        if (feof (stdin)) 
        {
            printf("\n");
            exit(0);
        }
        cmdline [ft_strlen(cmdline)-1] = '\0';
        eval (cmdline, envp);
        return (0);
    }
} */

int main(int argc, char **argv, char **envp) 
{
    char *cmdline;
    while (1) 
	{
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
    return (0);
}
