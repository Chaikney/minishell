/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:03:41 by chaikney          #+#    #+#             */
/*   Updated: 2024/05/31 16:03:42 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FIXME Too many functions in file.

// Return an executable path for cmd.
// NOTE cmd must be the name only, not any of its arguments.
// - split the pieces of PATH and add a trailing slash.
// - test the parts of path:
// -- does path + cmd = an executable?
// -- if YES we have our command: keep that and discard the rest.
// NOTE The return value of getenv("PATH") does not need to be freed
char	*search_in_path(char *cmd)
{
	char	**pathparts;
	char	*candidate;
	char	*slashed;
	char	*goodpath;
	int		i;

	i = 0;
	goodpath = NULL;
	pathparts = ft_split(getenv("PATH"), ':');
	while ((pathparts[i] != NULL) && (!goodpath))
	{
		slashed = ft_strjoin(pathparts[i], "/");
		candidate = ft_strjoin(slashed, cmd);
		if (access(candidate, X_OK) == 0)
			goodpath = ft_strdup(candidate);
		free (candidate);
		free(slashed);
		i++;
	}
	i = -1;
	while (pathparts[++i] != NULL)
		free(pathparts[i]);
	free(pathparts);
	return (goodpath);
}

// Look through a parsed set of commands and see if it contains
// flow control parameters: | > < >> or <<
// They would be separate from commands either side.
// FIXED This is PoC only: expand to other flow things
// (locates, it is up to the outer loop to re-identify what the thing is)
// DONE If not found, return -1. this can be if the reidrect is the last term
// ...it would be invalid in that position.
int	find_flow_control(t_command *cmdset)
{
	int	i;

	i = 0;
	while (i < cmdset->argc)
	{
		if (ft_strncmp(cmdset->argv[i], ">>", 2 == 0))
			break ;
		else if (ft_strncmp(cmdset->argv[i], "<<", 2 == 0))
			break ;
		else if (ft_strncmp(cmdset->argv[i], ">", 1 == 0))
			break ;
		else if (ft_strncmp(cmdset->argv[i], "<", 1 == 0))
			break ;
		else if (ft_strncmp(cmdset->argv[i], "|", 1 == 0))
			break ;
		i++;
	}
	if (i == cmdset->argc)
		i = -1;
	return (i);
}

// Find the path part (after the redirect >>)
// Open a file of that name for writing.
// Set the output to that file.
// run the command in a child process
// close file when done.
// FIXME This will not work without mangling the argvs before sending to run_
// NOTE The main (only?) difference from redirect_output is the O_APPEND flag
// TODO This could be combined with redirect: just set file options differently.
void	run_in_child_append_output(t_command *cmd, char **envp)
{
	int	out_file;
	int	redirect_posn;

	redirect_posn = find_flow_control(cmd);
	out_file = open(cmd->argv[redirect_posn + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (out_file == -1)
		perror("Could not open output file");
	dup2(out_file, STDOUT_FILENO);
	close (out_file);
	run_in_child(cmd, envp);
}

// Find the path part (after the redirect >)
// Open a file of that name for writing.
// Set the output to that file.
// run the command in a child process
// close file when done.
// FIXME This will not work without mangling the argvs
// TODO Think about how to find the needed parts
// TODO Do we need to reset STDOUT afterwards, or is this going to be process-limited?
void	run_in_child_redirect_output(t_command *cmd, char **envp)
{
	int	out_file;
	int	redirect_posn;

	redirect_posn = find_flow_control(cmd);
	out_file = open(cmd->argv[redirect_posn + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (out_file == -1)
		perror("Could not open output file");
	dup2(out_file, STDOUT_FILENO);
	close (out_file);
	run_in_child(cmd, envp);	// FIXME This will not work without mangling argvs!
}

// Make a child process to execute the command, putting the output in a pipe
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// TODO Can I use the output from this to measure state?
void	run_in_child_with_pipe(t_command *cmd, char **envp)
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
		waitpid(child, 0, 0);
		close(tube[0]);
	}
}

// Simplest command runner.
// Launches one command in a child process and waits for it to complete.
// NOTE This is the one we use for simple commands.
void	run_in_child(t_command *cmd, char **envp)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		exit_and_free(NULL, -1, -1);
	if (child == 0)
		run_command(cmd, envp);
	else
		waitpid(child, 0, 0);
}
